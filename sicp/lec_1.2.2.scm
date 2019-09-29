Lisp > (define fib (lambda (n)
  (cond ((= n 0) 0)
        ((= n 1) 1)
        (else (+ (fib (- n 1)) (fib (- n 2)))))))
fib
Lisp > (fib 5)
5
Lisp > (define fib (lambda (n) (fib-iter 1 0 n)))
fib
Lisp > (define fib-iter (lambda (a b count)
  (if (= count 0)
      b
      (fib-iter (+ a b) a (- count 1)))))
fib-iter
Lisp > (fib 5)
5
Lisp > (define count-change (lambda (amount)
  (cc amount 5)))
count-change
Lisp > (define cc (lambda (amount kinds-of-coins)
  (cond ((= amount 0) 1)
        ((or (< amount 0) (= kinds-of-coins 0)) 0)
        (else (+ (cc amount (- kinds-of-coins 1))
                 (cc (- amount (first-denomination kinds-of-coins)) kinds-of-coins)))))
)
cc
Lisp > (define first-denomination (lambda (kinds-of-coins)
  (cond ((= kinds-of-coins 1) 1)
        ((= kinds-of-coins 2) 5)
        ((= kinds-of-coins 3) 10)
        ((= kinds-of-coins 4) 25)
        ((= kinds-of-coins 5) 50))))
first-denomination
Lisp > (define or (lambda (x y)
  (cond ((= x true) true)
        ((= y true) true)
        (else false))))
or
Lisp > (count-change 100)
^C
