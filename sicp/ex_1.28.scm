Lisp > (define square (lambda (x) (* x x)))
square
Lisp > (define check-sqrt-1 (lambda (x m)
  (cond ((= x 1) 1)
        ((= x (- m 1)) 1)
        (else (if (= (mod (square x) m) 1)
                  0
                  (mod (square x) m))))))
check-sqrt-1
Lisp > (define even? (lambda (n)
  (= (mod n 2) 0)))
even?
Lisp > (define expmod (lambda (base ex m)
  (cond ((= ex 0) 1)
        ((even? ex) (check-sqrt-1
                       (expmod base (/ ex 2) m))
                       m)
        (else (mod (* base (expmod base (- ex 1) m))
                         m)))))
expmod
Lisp > (define try-it (lambda (a n)
  (= (expmod a (- n 1) n) 1)))
try-it
Lisp > (define miller-rabin-test (lambda (n)
  (try-it (+ 1 (random (- n 1))) n)))
miller-rabin-test
Lisp > (define fast-prime? (lambda (n times)
  (cond ((= times 0) true)
        ((miller-rabin-test n) (fast-prime? n (- times 1)))
        (else false))))
fast-prime?
Lisp > (define prime? (lambda (n) (fast-prime? n 20)))
prime?
Lisp > (prime? 561)
FALSE
Lisp > (prime?  1105 )
FALSE
Lisp > (prime?  1729 )
FALSE
Lisp > (prime?  2465)
FALSE
Lisp > (prime?  2821 )
FALSE
Lisp > (prime?  6601)
FALSE
