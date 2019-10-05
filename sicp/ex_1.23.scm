Lisp > (define next (lambda (n)
  (if (= n 2)
      3
      (+ n 2))))
next
Lisp > (define find-divisor (lambda (n test-divisor)
  (cond ((> (square test-divisor) n) n)
        ((divides? test-divisor n) test-divisor)
        (else (find-divisor n (next test-divisor))))))
find-divisor
Lisp > (define even? (lambda (n)
  (= (mod n 2) 0)))
even?
Lisp > (define timed-prime-test (lambda (n)
  (progn (newline)
         (display n)
         (start-prime-test n (runtime)))))
timed-prime-test
Lisp > (define start-prime-test (lambda (n start-time)
  (if (prime? n)
      (report-prime (- (runtime) start-time))
      ())))
start-prime-test
Lisp > (define report-prime (lambda (elapsed-time)
  (progn (display "*****")
         (display elapsed-time))))
report-prime
Lisp > (define search-for-primes (lambda (start end)
  (cond ((> start end) (progn (display "finished") (newline)))
        ((< start 2) (search-for-primes 2 end))
        (else (progn (timed-prime-test start)
                     (search-for-primes
                       (+ start (if (even? start) 1 2))
                       end))))))
search-for-primes
Lisp > (define prime? (lambda (n)
  (= n (smallest-divisor n))))
prime?
Lisp > (define smallest-divisor (lambda (n)
  (find-divisor n 2)))
smallest-divisor
Lisp > (define divides? (lambda (a b) (= (mod b a) 0)))
divides?
Lisp > (define square (lambda (x) (* x x)))
square
Lisp > (search-for-primes 1000 1030)

1000
1001
1003
1005
1007
1009*****809
1011
1013*****781
1015
1017
1019*****811
1021*****852
1023
1025
1027
1029finished
NIL
Lisp > (/ (+ 809 781 811) 3)
800.333313
Lisp > (search-for-primes 10000 10040)

10000
10001
10003
10005
10007*****3375
10009*****2943
10011
10013
10015
10017
10019
10021
10023
10025
10027
10029
10031
10033
10035
10037*****2863
10039*****3140finished
NIL
Lisp > (/ (+ 3375 2943 2863) 3)
3060.333252
Lisp > (search-for-primes 100000 100050)

100000
100001
100003*****16660
100005
100007
100009
100011
100013
100015
100017
100019*****14958
100021
100023
100025
100027
100029
100031
100033
100035
100037
100039
100041
100043*****15020
100045
100047
100049*****16177finished
NIL
Lisp > (/ (+ 16660 14958 15020) 3)
15546
Lisp > (search-for-primes 1000000 1000050)

1000000
1000001
1000003*****107074
1000005
1000007
1000009
1000011
1000013
1000015
1000017
1000019
1000021
1000023
1000025
1000027
1000029
1000031
1000033*****110664
1000035
1000037*****105777
1000039*****-894249
1000041
1000043
1000045
1000047
1000049finished
NIL
Lisp > (/ (+ 107074 110664 105777) 3)
107838.335938
Lisp > (/ 3060.333252 800.333313)
3.823823
Lisp > (/ 15546 3060.333252)
5.079840
Lisp > (/ 107838.335938 15546)
6.936725
