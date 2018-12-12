all:		otp_enc otp_enc_d otp_dec otp_dec_d

enc:		otp_enc otp_enc_d

dec:		otp_dec otp_dec_d

encode.o:	encode.h encode.c 
		gcc -c encode.c -o encode.o -Wall

otp_enc:	encode.o otp_enc.c
		gcc -o otp_enc otp_enc.c encode.o -Wall

otp_enc_d:	encode.o otp_enc_d.c
		gcc -o otp_enc_d otp_enc_d.c encode.o -Wall

decode.o:	decode.h decode.c
		gcc -c decode.c -o decode.o -Wall

otp_dec:	decode.o otp_dec.c
		gcc -o otp_dec otp_dec.c decode.o -Wall

otp_dec_d:	decode.o otp_dec_d.c
		gcc -o otp_dec_d otp_dec_d.c decode.o -Wall
