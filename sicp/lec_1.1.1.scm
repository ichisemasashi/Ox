Lisp > 486
486
Lisp > (+ 137 349)
486
Lisp > (- 1000 334)
666
Lisp > (* 5 99)
495
Lisp > (define / 
  (lambda (a b) (warisan a b 0)))
Lisp > 
/
(define warisan
  (lambda (a b count)
    (cond ((< a b) count)
          (else (warisan (- a b) b (+ count 1))))))
warisan
Lisp > (/ 10 5)
2
Lisp > (+ 2.7 10)
12.700000
Lisp > (+ 21 35 12 7)
75
Lisp > (* 25 4 12)
1200
Lisp > (+ (* 3 5) (- 10 6))
19
Lisp > (+ (* 3 (+ (* 2 4) (+ 3 5))) (+ (- 10 7) 6))
57
