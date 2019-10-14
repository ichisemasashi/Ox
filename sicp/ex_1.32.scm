Lisp > (define accumulate (lambda (combiner null-value term a next b)
  (if (> a b)
    null-value
    (combiner (term a)
              (accumulate combiner
                          null-value
                          term
                          (next a)
                          next
                          b)))))
accumulate
Lisp > (define accumulate (lambda (combiner null-value term a next b)
  (accumulate-iter combiner null-value term a next b)))
accumulate
Lisp > (define accumulate-iter (lambda (combiner null-value term a next b)
  (if (> a b)
    null-value
    (accumulate-iter combiner (combiner null-value (term a)) term (next a) next b))))
accumulate-iter
Lisp > (define sum (lambda (term a next b)
  (accumulate + 0 term a next b)))
sum
Lisp > (define product (lambda (term a next b)
  (accumulate * 1 term a next b)))
product
Lisp > (define square (lambda (x) (* x x)))
square
Lisp > (define pi-term (lambda (x)
    (/ (* x (+ x 2.0))
       (square (+ x 1)))))
pi-term
Lisp > (define pi-next (lambda (x) (+ x 2)))
pi-next
Lisp > (define pi-prod (lambda (n) (product pi-term 2 pi-next n)))
pi-prod
Lisp > (* 4 (pi-prod 1000))
3.143160
