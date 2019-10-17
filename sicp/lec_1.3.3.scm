Lisp > (define positive? (lambda (n) (< 0 n)))
positive?
Lisp > (define negative? (lambda (n) (< n 0)))
negative?
Lisp > (define average (lambda (x y) (/ (+ x y) 2)))
average
Lisp > (define abs (lambda (x)
  (cond ((< x 0) (- 0 x))
        (else x))))
abs
Lisp > (define and (lambda (x y)
  (cond ((= x true) (cond ((= y true) true)
                    (else false)))
        (else false))))
and
Lisp > (define cube (lambda (x) (* x x x)))
cube
Lisp > (define search (lambda (f neg-point pos-point)
  (let ((midpoint (average neg-point pos-point)))
    (if (close-enough? neg-point pos-point) midpoint
      (let ((test-value (f midpoint)))
        (cond ((positive? test-value)
               (search f neg-point midpoint))
              ((negative? test-value)
               (search f midpoint pos-point))
              (else midpoint)))))))
search
Lisp > (define close-enough? (lambda (x y) (< (abs (- x y)) 0.001)))
close-enough?
Lisp > (define half-interval (lambda (f a b)
  (let ((a-value (f a))
        (b-value (f b)))
    (cond ((and (negative? a-value) (positive? b-value))
           (search f a b))
          ((and (negative? b-value) (positive? a-value))
           (search f b a))
           (else "Opposite_sign")))))
half-interval
Lisp > (half-interval sin 2.0 4.0)
3.141113
Lisp > (half-interval (lambda (x) (- (* x x x) (* 2 x) 3))
                       1.0
                       2.0)
1.893066
Lisp > (define close-enough? (lambda (v1 v2)
    (< (abs (- v1 v2)) tolerance)))
close-enough?
Lisp > (define tolerance 0.00001)
tolerance
Lisp > (define fixed-point (lambda (f first-guess)
  (let ((try (lambda (guess)
    (let ((next (f guess)))
      (if (close-enough? guess next)
          next
          (try next))))))
  (try first-guess))))
fixed-point
Lisp > (fixed-point cos 1.0)
0.739082
Lisp > (fixed-point (lambda (y) (+ (sin y) (cos y))) 1.0)
1.258732
Lisp > (define sqrt (lambda (x)
  (fixed-point (lambda (y) (average y (/ x y)))
               1.0)))
sqrt
Lisp > (sqrt 81.0)
9.000000
Lisp > (define sqrt (lambda (x)
  (fixed-point (lambda (y) (/ x y))
               1.0)))
sqrt
Lisp > (sqrt 9.0)
zsh: segmentation fault  ./a.out
