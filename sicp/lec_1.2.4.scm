Lisp > (define expt (lambda (b n)
  (if (= n 0)
      1
      (* b (expt b (- n 1))))))
expt
Lisp > (expt 3 3)
27
Lisp > (expt 3 4)
81
Lisp > (define expt (lambda (b n) (expt-iter b n 1)))
expt
Lisp > (define expt-iter (lambda (b counter product)
  (if (= counter 0)
      product
      (expt-iter b (- counter 1) (* b product)))))
expt-iter
Lisp > (expt 3 3)
27
Lisp > (expt 3 4)
81
Lisp > (define fast-expt (lambda (b n)
  (cond ((= n 0) 1)
        ((even? n) (square (fast-expt b (/ n 2))))
        (else (* b (fast-expt b (- n 1)))))))
fast-expt
Lisp > (define even? (lambda (n)
  (= (remainder n 2) 0)))
even?
Lisp > (define square (lambda (x) (* x x)))
square
Lisp > (define remainder (lambda (a b) (rem a b 0 a)))
remainder
Lisp > (define rem
  (lambda (a b count org_a)
    (cond ((< a b) (- org_a (* b count)))
          (else (rem (- a b) b (+ count 1) org_a)))))
rem
Lisp > (fast-expt 3 3)
27
Lisp > (fast-expt 3 4)
81
