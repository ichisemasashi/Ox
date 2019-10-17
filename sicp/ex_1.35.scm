Lisp > (define abs (lambda (x)
  (cond ((< x 0) (- 0 x))
        (else x))))
abs
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
Lisp > (fixed-point (lambda (x) (+ 1 (/ 1 x))) 1.0)
1.618033
