Lisp > (define cube (lambda (x) (* x x x)))
cube
Lisp > (define sum (lambda (term a next b)
  (if (> a b)
      0
      (+ (term a)
         (sum term (next a) next b)))))
sum
Lisp > (define next- (lambda (n) (+ n 1)))
next-
Lisp > (define even? (lambda (x) (if (= (mod x 2) 0) true false)))
even?
Lisp > (define y (lambda (k f a h) (f (+ a (* k h)))))
y
Lisp > (define integral-simpson (lambda (f a b n)
  (let ((h (/ (- b a) n))
        (term- (lambda (i)
          (* (if (even? i) 2 4)
             (y i f a (/ (- b a) n))))))
    (* (/ h 3.0)
       (+ (y 0 f a h)
          (y n f a h)
          (sum term-
               1
               next-
               (- n 1)))))))
integral-simpson
Lisp > (integral-simpson cube 0 1 100)
442998.250000 ; expected 0.25
Lisp > (integral-simpson cube 0 1 1000)
454279648.000000 ; expected 0.25

