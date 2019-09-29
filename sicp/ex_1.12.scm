Lisp > (define pascals-triangle (lambda (n k)
  (if (or (= n k) (= k 1))
      1
      (+ (pascals-triangle (- n 1) (- k 1))
         (pascals-triangle (- n 1) k)))))
pascals-triangle
Lisp > (define or (lambda (x y)
  (cond ((= x true) true)
        ((= y true) true)
        (else false))))
or
Lisp > (pascals-triangle 7 1)
1
Lisp > (pascals-triangle 7 2)
6
Lisp > (pascals-triangle 7 3)
15
Lisp > (pascals-triangle 7 4)
20
Lisp > (pascals-triangle 7 5)
^C
