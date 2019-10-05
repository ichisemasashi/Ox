```
Lisp > true
TRUE
Lisp > false
FALSE
Lisp > (define plus (lambda (a b) (+ a b)))
plus
Lisp > (plus 2 3)
5
Lisp > (+ 1 2)
3
Lisp > (car (quote (1 2 3)))
1
Lisp > (cdr (quote (1 2 3)))
(2 3)
Lisp > (if (= 1 1) (quote 1) (quote 2))
1
Lisp > (define pi 3.14)
pi
Lisp > pi
3.140000
Lisp > (set pi 3.141592)
pi
Lisp > pi
3.141592
Lisp > (cons 1 (quote (a b c)))
(1 a b c)
Lisp > (= 1 2 3)
FALSE
Lisp > (= 1 1 1)
TRUE
Lisp > (< 0 1)
TRUE
Lisp > (cons 1 (cons 2 (cons 3 NIL)))
(1 2 3)
Lisp > (cons 1 ())
(1)
Lisp > (list 1 2 3)
(1 2 3)
Lisp > (cond ((= 1 2) (+ 1 2 3))
             ((< 1 2) 50)
             (else 8))
50
Lisp > (progn (+ 1 2) (* 2 3))
6
Lisp > (let ((x 1) (y 2)) (+ x y))
3
Lisp > (define my-add (lambda (x y z) (let ((ret (+ x y z))) ret)))
my-add
Lisp > (my-add 1 2 3)
6
Lisp > (progn (display "=====")(display (runtime))(newline))
=====660706
NIL
Lisp > (random 4)
4
Lisp > (random 4)
3
Lisp > (random 4)
2
```
