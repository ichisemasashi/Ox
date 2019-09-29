Lisp > (define abs (lambda (x)
  (cond ((< x 0) (- 0 x))
        (else x))))
(define sqrt-iter (lambda (guess x)
  (if (good-enough? guess x)
      guess
      (sqrt-iter (improve guess x) x))))
(define improve (lambda (guess x) (average guess (/ x guess))))
(define average (lambda (x y) (/ (+ x y) 2)))
(define good-enough? (lambda (guess x)
  (< (abs (- (square guess) x)) 0.001)))
(define sqrt (lambda (x) (sqrt-iter 1.0 x)))
(define square (lambda (x) (* x x)))
abs
Lisp > sqrt-iter
Lisp > improve
Lisp > average
Lisp > good-enough?
Lisp > sqrt
Lisp > square
Lisp > (sqrt 9.0)
3.000092
Lisp > (sqrt (+ 100.0 37.0))
11.704700
Lisp > (sqrt (+ (sqrt 2.0) (sqrt 3.0)))
1.773928
Lisp > (square (sqrt 1000.0))
1000.000427
