Lisp > (define double (lambda (n) (* n 2)))
double
Lisp > (define halve (lambda (n) (/ n 2)))
halve
Lisp > (define mul (lambda (a b)
  (cond ((= b 0) 0)
        ((even? b) (mul (double a) (halve b)))
        (else (+ (mul (double a) (halve (- b 1))) a)))))
mul
Lisp > (mul 3 4)
12
Lisp > (mul 9 9)
81
Lisp > (mul 1 1)
1
