Lisp > (define smallest-divisor (lambda (n)
  (find-divisor n 2)))
smallest-divisor
Lisp > (define find-divisor (lambda (n test-divisor)
  (cond ((> (square test-divisor) n) n)
        ((divides? test-divisor n) test-divisor)
        (else (find-divisor n (+ test-divisor 1))))))
find-divisor
Lisp > (define divides? (lambda (a b) (= (remainder b a) 0)))
divides?
Lisp > (define prime? (lambda (n)
  (= n (smallest-divisor n))))
prime?
Lisp > (prime? 10)
FALSE
Lisp > (define square (lambda (x) (* x x)))
square
Lisp > (prime? 11)
TRUE
Lisp > (define even? (lambda (n)
  (= (remainder n 2) 0)))
even?
Lisp > (define remainder (lambda (a b) (rem a b 0 a)))
remainder
Lisp > (define rem
  (lambda (a b count org_a)
    (cond ((< a b) (- org_a (* b count)))
          (else (rem (- a b) b (+ count 1) org_a)))))
rem
Lisp > (define square (lambda (x) (* x x)))
square
Lisp > (define random (lambda (n) (- n 2)))
random
Lisp > (define expmod (lambda (base ex m)
  (cond ((= ex 0) 1)
        ((even? ex) (remainder (square (expmod base (/ ex 2) m)) m))
        (else (remainder (* base (expmod base (- ex 1) m)) m)))))
expmod
Lisp > (define try-it (lambda (a n) (= (expmod a n n) a)))
try-it
Lisp > (define fermat-test (lambda (n) (try-it (+ 1 (random (- n 1))) n)))
fermat-test
Lisp > (define fast-prime? (lambda (n times)
  (cond ((= times 0) true)
        ((fermat-test n) (fast-prime? n (- times 1)))
        (else false))))
fast-prime?
Lisp > (fast-prime? 200 5)
Segmentation fault: 11
