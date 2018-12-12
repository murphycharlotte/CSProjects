CFLAGS = -Wall -fpic -coverage -lm -std=c99

tsp:
	g++ nn_final_325.cpp -o tsp 
	
example1: tsp
	./tsp tsp_example_1.txt 
	./tsp-verifier.py tsp_example_1.txt tsp_example_1.txt.tour

example2: tsp
	./tsp tsp_example_2.txt 
	./tsp-verifier.py tsp_example_2.txt tsp_example_2.txt.tour
	
example3: tsp
	./tsp tsp_example_3.txt 
	./tsp-verifier.py tsp_example_3.txt tsp_example_3.txt.tour
	
all_examples: tsp
	./tsp tsp_example_1.txt 
	./tsp-verifier.py tsp_example_1.txt tsp_example_1.txt.tour
	./tsp tsp_example_2.txt 
	./tsp-verifier.py tsp_example_2.txt tsp_example_2.txt.tour
	./tsp tsp_example_3.txt 
	./tsp-verifier.py tsp_example_3.txt tsp_example_3.txt.tour

test1: tsp
	./watch.py ./tsp test-input-1.txt 
	./tsp-verifier.py test-input-1.txt test-input-1.txt.tour
	
test2: tsp
	./watch.py ./tsp test-input-2.txt
	./tsp-verifier.py test-input-2.txt test-input-2.txt.tour

test3: tsp
	./watch.py ./tsp test-input-3.txt
	./tsp-verifier.py test-input-3.txt test-input-3.txt.tour

test4:  tsp
	./watch.py ./tsp test-input-4.txt
	./tsp-verifier.py test-input-4.txt test-input-4.txt.tour

test5:  tsp
	./watch.py ./tsp test-input-5.txt
	./tsp-verifier.py test-input-5.txt test-input-5.txt.tour

test6:  tsp
	./watch.py ./tsp test-input-6.txt
	./tsp-verifier.py test-input-6.txt test-input-6.txt.tour

test7:  tsp
	./watch.py ./tsp test-input-7.txt
	./tsp-verifier.py test-input-7.txt test-input-7.txt.tour
	
all_tests: tsp
	./watch.py ./tsp test-input-1.txt
	./tsp-verifier.py test-input-1.txt test-input-1.txt.tour
	./watch.py ./tsp test-input-2.txt
	./tsp-verifier.py test-input-2.txt test-input-2.txt.tour
	./watch.py ./tsp test-input-3.txt
	./tsp-verifier.py test-input-3.txt test-input-3.txt.tour
	./watch.py ./tsp test-input-4.txt
	./tsp-verifier.py test-input-4.txt test-input-4.txt.tour
	./watch.py ./tsp test-input-5.txt
	./tsp-verifier.py test-input-5.txt test-input-5.txt.tour
	./watch.py ./tsp test-input-6.txt
	./tsp-verifier.py test-input-6.txt test-input-6.txt.tour
	./watch.py ./tsp test-input-7.txt
	./tsp-verifier.py test-input-7.txt test-input-7.txt.tour
	
clean:
	rm -f tsp *.o *.tour