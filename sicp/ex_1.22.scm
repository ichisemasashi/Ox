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
Lisp > (define find-divisor (lambda (n test-divisor)
  (cond ((> (square test-divisor) n) n)
        ((divides? test-divisor n) test-divisor)
        (else (find-divisor n (+ test-divisor 1))))))
find-divisor
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
1009*****2105
1011
1013*****1280
1015
1017
1019*****1361
1021*****1406
1023
1025
1027
1029finished
NIL
Lisp > (search-for-primes 10000 10040)

10000
10001
10003
10005
10007*****6512
10009*****5028
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
10037*****6055
10039*****6235finished
NIL
Lisp > (search-for-primes 100000 100050)

100000
100001
100003*****37234
100005
100007
100009
100011
100013
100015
100017
100019*****35610
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
100043*****38967
100045
100047
100049*****37118finished
NIL
Lisp > (search-for-primes 1000000 1000050)

1000000
1000001
1000003*****-674413
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
1000033*****334482
1000035
1000037*****333834
1000039*****-666600
1000041
1000043
1000045
1000047
1000049finished
NIL
