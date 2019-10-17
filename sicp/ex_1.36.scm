Lisp > (define abs (lambda (x)
  (cond ((< x 0) (- 0 x))
        (else x))))
abs
Lisp > (define close-enough? (lambda (v1 v2)
    (< (abs (- v1 v2)) tolerance)))
close-enough?
Lisp > (define tolerance 0.00001)
tolerance
Lisp > (define try (lambda (f guess)
  (let ((next (f guess)))
    (progn
       (display next)
       (newline)
       (if (close-enough? guess next)
           next
           (try f next))))))
try
Lisp > (define fixed-point (lambda (f first-guess)
  (try f first-guess)))
fixed-point
Lisp > (fixed-point (lambda (x) (/ (log 1000) (log x))) 2)
9.965784
3.004472
6.279196
3.759851
5.215844
4.182207
4.827765
4.387594
4.671250
4.481404
4.605366
4.523085
4.577115
4.541383
4.564903
4.549373
4.559607
4.552854
4.557305
4.554369
4.556305
4.555028
4.555871
4.555315
4.555681
4.555440
4.555599
4.555494
4.555563
4.555518
4.555548
4.555528
4.555541
4.555532
4.555532
Lisp > (define average (lambda (x y) (/ (+ x y) 2.0)))
average
Lisp > (fixed-point (lambda (x) (average x (/ (log 1000) (log x)))) 2.0)
5.982892
4.922169
4.628224
4.568347
4.557731
4.555910
4.555599
4.555547
4.555538
4.555538
