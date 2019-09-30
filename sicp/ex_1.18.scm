Lisp > (define mul (lambda (a b) (mul-iter a b 0)))
mul
Lisp > (define mul-iter (lambda (a b n)
  (cond ((= b 0) n)
        ((even? b) (mul-iter (double a) (halve b) n))
        (else (mul-iter a (- b 1) (+ a n))))))
mul-iter
Lisp > (mul 2 4)
8
