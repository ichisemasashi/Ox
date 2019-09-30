Lisp > (define fast-expt-iter (lambda (b n)
  (expt-iter b n 1)))
fast-expt-iter
Lisp > (define expt-iter (lambda (b n a)
  (cond ((= n 0) a)
        ((even? n) (expt-iter (* b b) (/ n 2) a))
        (else (expt-iter b (- n 1) (* b a))))))
expt-iter
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
Lisp > (fast-expt-iter 2 3)
8
