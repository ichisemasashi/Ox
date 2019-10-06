Lisp > 	(define next (lambda (n)
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
Lisp > (define fool-fermat-test? (lambda (n) (fool-iter (- n 1) n)))
fool-fermat-test?
Lisp > (define fool-iter (lambda (a n)
  (cond ((= a 0) true)
        ((= (expmod a n n) a) (iter (- a 1)))
        (else false))))
fool-iter
Lisp > (fool-fermat-test? 561)
(iter 559)
Lisp > (define fool-iter (lambda (a n)
  (cond ((= a 0) true)
        ((= (expmod a n n) a) (iter (- a 1) n))
        (else false))))
fool-iter
Lisp > (fool-fermat-test? 561)
(iter 559 561)
Lisp > (define fool-iter (lambda (a n)
  (cond ((= a 0) true)
        ((= (expmod a n n) a) (fool-iter (- a 1) n))
        (else false))))
fool-iter
Lisp > (fool-fermat-test? 561)
TRUE
Lisp > (fool-fermat-test? 1105)
TRUE
Lisp > (fool-fermat-test? 1729)
TRUE
Lisp > (fool-fermat-test? 2465)
TRUE
Lisp > (fool-fermat-test? 2821)
TRUE
Lisp > (fool-fermat-test? 6601)
Segmentation fault: 11
