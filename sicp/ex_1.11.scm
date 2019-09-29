Lisp > (define f (lambda (n)
  (if (< n 3)
      n
      (+ (f (- n 1))
         (* 2 (f (- n 2)))
         (* 3 (f (- n 3)))))))
f
Lisp > (f 3)
4
Lisp > (f 8)
335
Lisp > (define f (lambda (n)
  (if (< n 3)
      n
      (f-iter 2 1 0 n))))
f
Lisp > (define f-iter (lambda (a b c n)
  (if (= n 2)
      a
      (f-iter (+ a (* 2 b) (* 3 c)) a b (- n 1)))))
f-iter
Lisp > (f 4)
11
Lisp > (f 3)
4
Lisp > (f 8)
335
