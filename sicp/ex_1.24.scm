Lisp > (define next (lambda (n)
  (if (= n 2)
      3
      (+ n 2))))
next
Lisp > (define fast-prime? (lambda (n times)
  (cond ((= times 0) true)
        ((fermat-test n) (fast-prime? n (- times 1)))
        (else false))))
fast-prime?
Lisp > (define fermat-test (lambda (n) (try-it (+ 1 (random (- n 1))) n)))
fermat-test
Lisp > (define try-it (lambda (a n) (= (expmod a n n) a)))
try-it
Lisp > (define expmod (lambda (base ex m)
  (cond ((= ex 0) 1)
        ((even? ex) (mod (square (expmod base (/ ex 2) m)) m))
        (else (mod (* base (expmod base (- ex 1) m)) m)))))
expmod
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
  (if (fast-prime? n 1)
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
1009*****736
1011
1013*****950
1015
1017
1019*****835
1021*****711
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
10007*****1018
10009*****925
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
10037*****882
10039*****947finished
NIL
Lisp > (search-for-primes 100000 100050)

100000
100001
100003
100005
100007
100009
100011
100013
100015
100017
100019
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
100043
100045
100047
100049finished
NIL
Lisp > (search-for-primes 100000 100050)

100000
100001
100003
100005
100007
100009
100011
100013
100015
100017
100019
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
100043
100045
100047
100049finished
NIL
Lisp > (search-for-primes 100000 100050)

100000
100001
100003
100005
100007
100009
100011
100013
100015
100017
100019
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
100043
100045
100047
100049finished
NIL
