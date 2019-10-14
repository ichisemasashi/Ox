Lisp > (define identity (lambda (x) x))
identity
Lisp > (define inc (lambda (n) (+ n 1)))
inc
Lisp > (define sum (lambda (term a next b)
  (let ((iter (lambda (a result)
    (if (> a b)
      result
      (iter (next a)
            (+ (term a) result))))))
  (iter a 0))))
sum
Lisp > (sum identity 1 inc 3)
6
