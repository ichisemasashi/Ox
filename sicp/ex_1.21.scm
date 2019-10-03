Lisp > (define smallest-divisor (lambda (n) (find-divisor n 2)))
smallest-divisor
Lisp > (define find-divisor (lambda (n test-divisor)
  (cond ((> (square test-divisor) n) n)
        ((divides? test-divisor n) test-divisor)
        (else (find-divisor n (+ test-divisor 1))))))
find-divisor
Lisp > (define divides? (lambda (a b) (= (remainder b a) 0)))
divides?
Lisp > (define square (lambda (x) (* x x)))
square
Lisp > (define remainder (lambda (a b) (rem a b 0 a)))
remainder
Lisp > (define rem
  (lambda (a b count org_a)
    (cond ((< a b) (- org_a (* b count)))
          (else (rem (- a b) b (+ count 1) org_a)))))
rem
Lisp > (smallest-divisor 199)
199
Lisp > (smallest-divisor 1999)
1999
Lisp > (smallest-divisor 19999)
7
