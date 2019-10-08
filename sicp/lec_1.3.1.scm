Lisp > (define sum-integers (lambda (a b)
  (if (> a b)
      0
      (+ a (sum-integers (+ a 1) b)))))
sum-integers
Lisp > (sum-integers 1 10)
55
Lisp > (define cube (lambda (x) (* x x x)))
cube
Lisp > (define sum-cubes (lambda (a b)
  (if (> a b)
      0
      (+ (cube a)
         (sum-cubes (+ a 1) b)))))
sum-cubes
Lisp > (sum-cubes 1 10)
3025
Lisp > (define pi-sum (lambda (a b)
  (if (> a b)
      0
      (+ (/ 1.0 (* a (+ a 2)))
         (pi-sum (+ a 4) b)))))
pi-sum
Lisp > (* 8 (pi-sum 1 1000))
3.139593
Lisp > (define sum (lambda (term a next b)
  (if (> a b)
      0
      (+ (term a)
         (sum term (next a) next b)))))
sum
Lisp > (define inc (lambda (n) (+ n 1)))
inc
Lisp > (define sum-cubes (lambda (a b) (sum cube a inc b)))
sum-cubes
Lisp > (sum-cubes 1 10)
3025
Lisp > (define identity (lambda (x) x))
identity
Lisp > (define sum-integers (lambda (a b)
  (sum identity a inc b)))
sum-integers
Lisp > (sum-integers 1 10)
55
Lisp > (define pi-term (lambda (x) (/ 1.0 (* x (+ x 2)))))
pi-term
Lisp > (define pi-next (lambda (x) (+ x 4)))
pi-next
Lisp > (define pi-sum (lambda (a b) (sum pi-term a pi-next b)))
pi-sum
Lisp > (* 8 (pi-sum 1 1000))
3.139593
Lisp > (define add-dx (lambda (x) (+ x 0.001)))
add-dx
Lisp > (define integral (lambda (f a b dx) 
  (* (sum f (+ a (/ dx 2.0)) add-dx b) dx)))
integral
Lisp > (integral cube 0 1 0.001)
0.249994
