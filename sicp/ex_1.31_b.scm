Lisp > (define product-iter (lambda (term a next b result)
  (if (> a b) result
      (product-iter term (next a) next b (* (term a) result)))))
product-iter
Lisp > (define product (lambda (term a next b)
  (product-iter term a next b 1)))
product
Lisp > (define inc (lambda (n) (+ n 1)))
inc
Lisp > (define identity (lambda (x) x))
identity
Lisp > (define factorial (lambda (n)
  (product identity 1 inc n)))
factorial
Lisp > (factorial 5)
120
Lisp > (factorial 100)  
0
Lisp > (factorial 15)   
2004310016
