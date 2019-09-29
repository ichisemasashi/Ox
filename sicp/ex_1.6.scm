Lisp > (define new-if (lambda (predicate then-clause else-clause)
  (cond (predicate then-clause)
        (else else-clause))))
new-if
Lisp > (new-if (= 2 3) 0 5)
5
Lisp > (new-if (= 2 2) 0 5)    
0
Lisp > (define abs (lambda (x)
  (cond ((< x 0) (- 0 x))
        (else x))))
abs
Lisp > (define improve (lambda (guess x) (average guess (/ x guess))))
improve
Lisp > (define average (lambda (x y) (/ (+ x y) 2)))
average
Lisp > (define good-enough? (lambda (guess x)
  (< (abs (- (square guess) x)) 0.001)))
good-enough?
Lisp > (define sqrt (lambda (x) (sqrt-iter 1.0 x)))
sqrt
Lisp > (define square (lambda (x) (* x x)))
square
Lisp > (define sqrt-iter (lambda (guess x)
  (new-if (good-enough? guess x)
          guess
          (sqrt-iter (improve guess x) x))))
sqrt-iter
Lisp > (sqrt 9.0)
^C
