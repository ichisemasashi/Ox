Lisp > (define cube (lambda (x) (* x x x)))
cube
Lisp > (define square (lambda (x) (* x x)))
square
Lisp > (define improve (lambda (guess x)
  (/ (+ (/ x (square guess)) (* guess 2.0))
     3.0)))
improve
Lisp > (define cube-root-iter (lambda (old-guess new-guess x)
  (if (good-enough? old-guess new-guess)
      new-guess
      (cube-root-iter new-guess (improve new-guess x) x))))
cube-root-iter
Lisp > (define good-enough? (lambda (old-guess new-guess)
  (< (abs (- 1.0 (/ old-guess new-guess))) 0.001)))
good-enough?
Lisp > (define cube-root (lambda (x) (cube-root-iter 1.0 x x)))
cube-root
Lisp > (cube-root 27.0)
3.000000
Lisp > (cube-root 27)
3.000000
