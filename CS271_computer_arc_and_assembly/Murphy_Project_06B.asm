TITLE Combinations Calculator     (Murphy_Project_06B.asm)

; Author:		Charlotte Murphy
; Course:		CS 271-400
; Project ID:	Assignment #6B	                
; Due date:	March 19, 2017
; Description:	This program generates practice combinatorics problems, Asks the user for an answer
;	displays the results & if the answer was correct, and asks the user if another problem should
;	be generated.

INCLUDE Irvine32.inc

; Macro code to display a string 
; Source: lecture 26
mWriteString	MACRO	buffer
	push	edx				;Save edx register
	mov	edx, buffer
	call	WriteString
	pop	edx				;Restore edx
ENDM

;Since the HI and LO values don't change for n and r,
;I created global constants for the RANGE values to avoid
;calculating the SAME number every time showProblem is called.
HI = 12
LO_R = 1
LO_N = 3
RANGE_N = 10


.data
intro_1		BYTE		"Welcome to the Combinations Calculator   Implemented by Charlotte Murphy ", 0 
intro_2		BYTE		"I will give you a combinations problem. You enter your answer. I will let you know if you are right. ", 0
ec_1			BYTE		"**EC: Extra Credit 1. ", 0
ec_2			BYTE		"**EC: Extra Credit 1. ", 0
problem		BYTE		"Problem: ", 0
problem_n		BYTE		"Number of elements in the set: ", 0
problem_r		BYTE		"Number of elements to choose from the set: ", 0
get_answer	BYTE		"How many ways can you choose? ", 0  
result_1		BYTE		"There are ", 0
result_2		BYTE		" combinations of ", 0
result_3		BYTE		" items from a set of ", 0 
correct		BYTE		"You are connrect! ", 0
incorrect		BYTE		"You need more practice. ", 0
repeat_prob	BYTE		"Another problem? (y/n): ", 0
input_error	BYTE		"Invalid input. ", 0
farewell_1	BYTE		"OK ... Goodbye. ", 0

n			DWORD	?
r			DWORD	?
is_valid		DWORD	0
answer_str	BYTE		32 DUP (?)
answer_int	DWORD	?
result		DWORD	?
again		BYTE		10 DUP (?)
new_prob		DWORD	0

.code
main PROC

	call		Randomize

	push		OFFSET intro_1
	push		OFFSET intro_2
	push		OFFSET ec_1
	push		OFFSET ec_2
	call		intro

beginning:

	push		OFFSET problem
	push		OFFSET problem_n
	push		OFFSET n
	push		OFFSET problem_r
	push		OFFSET r
	call		showProblem

	push		OFFSET get_answer
	push		OFFSET answer_str
	push		OFFSET answer_int
	push		OFFSET is_valid
	call		getData

	push		n
	push		r
	push		OFFSET result
	call		combinations

	push		answer_int
	push		result
	push		n
	push		r
	call		showResults

	push		OFFSET new_prob
	push		OFFSET repeat_prob
	push		OFFSET again
	call		another

	call		Crlf

	cmp		new_prob, 1
	je		beginning

	exit	; exit to operating system
main ENDP

;***************************************************************************************
; intro
; Procedure to introduce the program.
; Receives parameters on the system stack (in the order pushed): 
;	OFFSET intro_1
;	OFFSET intro_2
;	OFFSET ec_1
;	OFFSET ec_2
; returns: none
; preconditions:  none
; registers changed: none
;***************************************************************************************
intro		PROC
	
	;Save registers changed and set up stack frame
	push		eax
	push		ebp				
	mov		ebp, esp

	;Display program title
	mWriteString	 [ebp + 24]
	call		Crlf

	;Display extra credit options
	mWriteString	 [ebp + 16]
	call		Crlf
	mWriteString	 [ebp + 12]
	call		Crlf

	;Display program description
	mWriteString	 [ebp + 20]
	call		Crlf
	call		Crlf

	;restore registers
	pop		ebp
	pop		eax

	;return and pop 4 parameters off of stack
	ret		16
intro	ENDP

;***************************************************************************************
; showProblem
; procedure to generate random numbers for and display a combinations statistics problem.
;	generates n within [LO-N .. HI]
;	generates r within [1 .. n]
; Receives parameters on the system stack (in the order pushed): 
;	OFFSET problem -  address of string
;	OFFSET problem_n - address of string
;	OFFSET n	- address of n
;	OFFSET problem_r - address of string
;	OFFSET r - address of r
; returns: none
; preconditions: Randomize is called in main
; labels changed (by reference): n, r
;***************************************************************************************
showProblem	PROC
	
	;save registers and set up stack frame
	push		eax		;8
	push		ebx		;4
	push		ebp		;0
	mov		ebp, esp

	;Display problem
	mWriteString	 [ebp + 32]
	call		Crlf
	

	;Display problem_n
	mWriteString	 [ebp + 28]

	;generate n
	mov		eax, RANGE_N
	call		RandomRange
	add		eax, LO_N

	;Update 'n' value
	mov		ebx, [ebp + 24]		;ebx now == address of 'n'
	mov		[ebx], eax			;mov eax to value stored at address of 'n'

	;display n at the end of problem_n string
	call		WriteDec
	call		Crlf

	
	;Display problem_r
	mWriteString	 [ebp + 20]

	;generate r
	;r is in [ 1 .. n], and eax still == n.
	;so decrement eax to get range [0 .. n-1]
	;generate random r and save to eax
	;then increment so eax r is back in [ 1 .. n]
	dec		eax
	call		RandomRange
	inc		eax

	;Update 'r' value
	mov		ebx, [ebp + 16]		;ebx now == address of 'r'
	mov		[ebx], eax			;mov eax to value stored at address of 'r'

	;display r at the end of problem_r string
	call		WriteDec

	;restore registers
	pop		ebp
	pop		ebx
	pop		eax

	;resturn and pop 5 elements off stack
	ret		20
showProblem	ENDP

;***************************************************************************************
; getData
; Procedure to request and store user input per instructions displayed in showProblem PROC
;	Stores user input as a string, calls stringToInt PROC to convert string to an integer.
;	After call to stringToInt PROC, if is_valid == 0, input is repeated.
;	If is_valid == 1, getData returns
; Subprocedure, stringToInt, is called to convert string to an integer, if possible,
;	and set "is_valid' to 1 or 0, respectively. 
; Receives parameters on the system stack (in the order pushed): 
;	'get_answer' (reference) - user prompt
;	'answer_str' (reference) - stores user input string
;	'answer_int' (reference) - stores numeric value of 'answer_str'
;	'is_valid' (reference) - getData passes "is_valid' by reference to stringToInt PROC
; returns: none
; preconditions: n is in [LO_N .. HI]
;			  r is in [1 .. n]
; labels changed (by reference): 'answer_str'
; registers changed: none
;***************************************************************************************
getData		PROC
	
	;save registers and set up stack frame
	push		eax			;16
	push		ebx			;12
	push		ecx			;8
	push		edx			;4
	push		ebp			;0			
	mov		ebp, esp	

	call		CrLf

top:

	;reset 'answer_int' value to 0 before each problem
	mov		eax, [ebp + 28]
	mov		ebx, 0
	mov		[eax], ebx

	;display 'get_answer' and store user input in 'answer'
	mWriteString	[ebp + 36]
	mov		edx, [ebp + 32]
	mov		ecx, 10
	call		ReadString
	
	call		CrLf

	;ebx gets answer_int offset
	;ecx gets is_valid offset
	mov		ebx,	[ebp + 28]
	mov		ecx,	[ebp + 24]

	;push	OFFSET answer_str	
	;push	OFFSET answer_int	
	;push	string length		
	;push	offset is_valid	
	push		edx
	push		ebx
	push		eax
	push		ecx
	call		stringToInt

	;compare 'iv_valid' to 1
	;ecx still holds 'is_valid' offset
	;eax gets value of 'is_valid'
	mov		eax, [ecx]
	cmp		eax, 1
	jne		top
	;if not equal, jump to top

	;restore registers
	pop		ebp
	pop		edx
	pop		ecx
	pop		ebx
	pop		eax

	;return and pop 4 elements off stack
	ret		16
getData		ENDP

;***************************************************************************************
; stringToInt
; Procedure to convert individual characters of a string to their respective numeric
;	values, 0 - 9.
;	If the string contains non-numeric characters, 'is_valid' is set to 0.
;	Else, 'is_valid' is set to 1
; In addition to sources listed below, I referenced deom6.asm and lecture 23
; Receives parameters on the system stack (in the order pushed): 
;	'answer_str' (reference)
;	'answer_int' (reference)
;	length of string (value)
;	'is_valid' (reference)
; Returns: none
; Preconditions: 'answer_str' in not NULL
;			  length is > 0
; labels changed (by reference): 'answer_int' and 'is_valid'
; registers changed: none
;***************************************************************************************
stringToInt	PROC

	;save registers and set up stack frame
	push		eax				;20
	push		ebx				;16
	push		ecx				;12
	push		esi				;8
	push		edi				;4
	push		ebp				;0
	mov		ebp, esp

	;set up loop counter
	;esi gets answer_str offset
	;edi gets answer_int offset
	;clear direction flag
	mov		esi, [ebp + 40]
	mov		edi, [ebp + 36]
	mov		ecx, [ebp + 32]
	cld

L1:
	;eax gets answer_int value
	;mutliply by eax 10 and 
	;update answer_int value
	mov		eax, [edi]
	mov		ebx, 10
	mul		ebx
	mov		[edi], eax

	;load char at [esi] to al
	;validate if char is an int
	;if not an int, jump to notInt:
	lodsb
	cmp		al, 48
	jb		notInt
	cmp		al, 57
	ja		notInt

	;else, subtract 48 from al and 
	;add al to answer_int
	sub		al, 48
	add		[edi], al

	;loop through entire string
	loop		l1

	;set 'is_valid' to 1
	mov		eax, [ebp + 28]
	mov		ebx, 1
	mov		[eax], ebx 

	;jump to end of proc
	jmp		bottom

notInt:
	;display input error
	mWriteString	OFFSET input_error

	;set 'is_valid' to 0
	mov		eax, [ebp + 28]
	mov		ebx, 0
	mov		[eax], ebx 

bottom:
	;restore registers
	pop		ebp
	pop		edi
	pop		esi
	pop		ecx
	pop		ebx
	pop		eax

	;return and pop 4 elements of stocks
	ret		16
stringToInt	ENDP

;***************************************************************************************
; combinations
; Procedure to perform calculations necesary for combination problem nCr
; nCr = n! / r! (n - r)!
; Subprocedure: factorial is called three times to calculate n!, r!, and (n-r)!
; Receives parameters on the system stack (in the order pushed): 
;	'n' - (value)
;	'r' - (value)
;	'result' - (reference)
; Returns: none
; preconditions: 'n' is in [LO_N .. HI]
;			  'r' is in [1 .. n]
; labels changed (by reference): 'result';	
; registers changed: none
;***************************************************************************************
combinations	PROC

	;Save registers changed and set up stack frame
	push		eax		;12
	push		ebx		;8
	push		edi		;4
	push		ebp		;0
	mov		ebp, esp

	;edi gets offset of result
	mov		edi, [ebp + 20]	

	;ebx gets value of 'r'
	mov		ebx, [ebp + 24]

	;find r!
	;store in result
	push		ebx
	call		factorial
	mov		[edi], eax

	;find (n-r)
	;ebx still == r
	mov		eax, [ebp + 28]
	sub		eax, ebx
	;find (n-r)!
	push		eax
	call		factorial
	;eax == (n-r)!
	;ebx gets value of result, which still == r!
	mov		ebx, [edi]
	;r! * (n-r)1
	mul		ebx
	;store product in result 
	mov		[edi], eax


	;eax gets n
	;find n!
	mov		eax, [ebp + 28]
	push		eax
	call		factorial
	;eax gets n!
	;ebx gets (r!(n-r!))
	mov		ebx, [edi]
	;div ebx/eax 
	;store quotient in 'result'
	div		ebx
	mov		[edi], eax

	;restore registers
	pop		ebp
	pop		edi
	pop		ebx
	pop		eax

	;return and pop 3 elements off of stack
	ret		12
combinations	ENDP

;***************************************************************************************
; factorial
; Procedure to calculate the factorial of an unsigned integer (x!)
; Receives parameters on the system stack (in the order pushed): 
;	an unsigned integer
; Returns: the value of x! in eax
; Preconditions: parameter passed is an unsigned integer
; Labels changed: none
; Registers changed: eax
;***************************************************************************************
factorial		PROC

	;Save registers changed and set up stack frame
	push		ebp
	mov		ebp, esp

	;eax gets value of n
	;if n <= 1, jump to quit
	mov		eax, [ebp + 8]
	cmp		eax, 1
	jle		quit

	;else, decrement eax
	;pass eax to next call of factorial
	dec		eax
	push		eax
	call		factorial

	;eax contains product returned from previous call of factorial
	;ebx gets n
	;multiply ebx and eax
	mov		ebx, [ebp + 8]
	mul		ebx

quit:
	;restore ebp
	pop		ebp

	;return product in eax
	ret		4
factorial		ENDP

;***************************************************************************************
; showResults
; Procedure to calculate the display the results of a combinations problem (nCr = ?)
;	and to assess & provide feedback for user's anser/input
; Receives parameters on the system stack (in the order pushed): 
;	'answer_int' - (value)
;	'result' - (value)
;	'r' - (value)
;	'n' - (value)
; Returns: none
; Labels changed: none
; Registers changed: none
;***************************************************************************************
showResults	PROC

	;save registers and set up stack frame
	push		eax		;4
	push		ebp		;0			
	mov		ebp, esp
	
	;Display results strings and values all in one line		
	mWriteString	OFFSET result_1
	;display 'result' 
	mov		eax, [ebp + 20]
	call		WriteDec
	mWriteString	OFFSET result_2
	;display 'r' 
	mov		eax, [ebp + 16]
	call		WriteDec
	mWriteString	OFFSET result_3
	;display 'n' 
	mov		eax, [ebp + 12]
	call		WriteDec
	call		Crlf

	;compare answer and result
	;and display appropriate feedback
	;eax gets value of 'answer'
	;ebx gets value of 'result'
	mov		eax, [ebp + 24]
	mov		ebx, [ebp + 20]
	
	;if answer == result, display 'correct'
	.IF		eax == ebx
		mWriteString	OFFSET correct
		jmp		show_last
	;else display 'incorrect'
	.ELSE
		mWriteString	OFFSET incorrect
	.ENDIF

show_last:
	;line breaks for formatting
	call		Crlf
	call		Crlf

	;restore registers
	pop		ebp
	pop		eax

	;return and pop 4 elements of stack
	ret		16
showResults	ENDP

;***************************************************************************************
; another
; Procedure to determine if user wants to repeat the program. It displays a prompt, 'repeat_prob'
;	asking the user to enter 'y' to continue or 'n' to stop. The procedure only stores and 
;	validates the first character entered in 'again'. It then verifies that the stored character
;	is either 'y', 'Y', 'n', or 'N'. If input is not one of those values, and error 
;	message is displayed, and input is repeated.
;	Once input is validated, the value is assessed, and 
;	if input == 'y' or 'Y', 'new_prob' gets 1
;	else, 'new_prob' gets 0
; Receives parameters on the system stack (in the order pushed): 
;	'new_prob' - (reference)
;	'repeat_prob' - (reference)
;	'again' - (reference)
; Returns: none
; Preconditions: none
; Labels changed: none
; Registers changed (by reference): 'again' and 'new_prob'
;***************************************************************************************
another	PROC

	;save registers and set up stack frame
	push		eax		;12
	push		edx		;8
	push		esi		;4
	push		ebp		;0
	mov		ebp, esp

first:
	;display 'repeat_prob' and store user input in 'again'
	;only store first character of input
	mWriteString	[ebp + 24] 
	mov		edx, [ebp + 20]
	mov		ecx, 2
	call		ReadString
	call		CrLf

	;if string == 'y' or 'Y', jump to another_prob
	;else if string == 'n' or 'N' jump to last_prob
	mov		eax, [edx]
	cmp		eax, 121			;121 == 'y'
	je		another_prob
	cmp		eax, 89			;89 == 'Y'
	je		another_prob
	cmp		eax, 110			;110 == 'n'
	je		last_prob
	cmp		eax, 78			;78 == 'N'
	je		last_prob

	;else display error and jumpt to top and repeat input
	mWriteString	OFFSET input_error
	;call		CrLf
	jmp		first

;if again == 'y' or 'Y'
another_prob:
	;set 'new_prob' to 1
	mov		eax, [ebp + 28]
	mov		ebx, 1
	mov		[eax], ebx
	jmp		last

;if again == 'n' or 'N'
last_prob:
	;set 'new_prob' to 0
	mov		eax, [ebp + 28]
	mov		ebx, 0
	mov		[eax], ebx

	;display farewell message
	mWriteString	OFFSET farewell_1

last:
	;restore registers
	pop		ebp
	pop		esi
	pop		edx
	pop		eax

	;return and pop 3 elements off stack
	ret		12

another	ENDP

END main


; Additional sources
;	http://stackoverflow.com/questions/13666153/masm-convert-string-to-integer-processing-invalid-input
;	www.cs.cornell.edu/courses/cs2110/2012sp/lectures/05-Recursion_6up.pdf 

