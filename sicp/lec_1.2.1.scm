Lisp > (define factorial (lambda (n)
  (if (= n 1)
      1
     (* n (factorial (- n 1))))))
factorial
Lisp > (factorial 6)
720
Lisp > (define factorial (lambda (n) (fact-iter 1 1 n)))
factorial
Lisp > (define fact-iter (lambda (product counter max-count)
  (if (> counter max-count)
      product
      (fact-iter (* counter product)
                 (+ counter 1)
                 max-count))))
fact-iter
Lisp > (factorial 6)
720
