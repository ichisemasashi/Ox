Lisp > 10
10
Lisp > (+ 5 3 4)
12
Lisp > (- 9 1)
8
Lisp > (define / 
  (lambda (a b) (warisan a b 0)))
/
Lisp > (define warisan
  (lambda (a b count)
    (cond ((< a b) count)
          (else (warisan (- a b) b (+ count 1))))))
warisan
Lisp > (/ 6 2)
3
Lisp > (+ (* 2 4) (- 4 6))
6
Lisp > (define a 3)
a
Lisp > (define b (+ a 1))
b
Lisp > (+ a b (* a b))
19
Lisp > (= a b)
FALSE
Lisp > (if (and (> b a) (< b (* a b)))
b a)
4
Lisp > (cond ((= a 4) 6)
      ((= b 4) (+ 6 7 a))
      (else 25))
16
Lisp > (+ 2 (if (> b a) b a))
6
Lisp > (* (cond ((> a b) a) ((< a b) b) 
         (else -1))
   (+ a 1))
16
