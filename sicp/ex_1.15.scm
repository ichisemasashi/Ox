Lisp > (define cube (lambda (x) (* x x x)))
cube
Lisp > (define p (lambda (x) (- (* 3 x) (* 4 (cube x)))))
p
Lisp > (define sine (lambda (angle)
  (if (not (> (abs angle) 0.1)) angle
      (p (sine (/ angle 3.0))))))
sine
Lisp > (define abs (lambda (x)
  (cond ((< x 0) (- 0 x))
        (else x))))
abs
Lisp > (define not (lambda (x)
  (cond ((= x true) false)
        (else  true))))
not
Lisp > (sine 12.15)
-0.399803
