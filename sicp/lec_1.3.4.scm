(define square (lambda (x) (* x x)))
(define average (lambda (x y) (/ (+ x y) 2)))
(define average-damp (lambda (f)
  (lambda (x) (average x (f x)))))
((average-damp square) 10)
; 期待値は55

