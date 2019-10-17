Lisp > (define cont-frac (lambda (n d k)
  (let ((iter (lambda (i)
    (if (= i k)
        (/ (n i) (d i))
        (/ (n i) (+ (d i) (iter (+ i 1))))))))
  (iter 1))))
cont-frac
Lisp > (define iter-a-to-b (lambda (f a b)
  (progn
    (newline)
    (display a)
    (display "->")
    (if (> a b)
        (f a)
        (progn (display (f a)) (iter-a-to-b f (+ a 1) b))))))
iter-a-to-b
Lisp > (define tan-cf (lambda (x k)
  (cont-frac (lambda (i) (if (= i 1) x (- (square x))))
             (lambda (i) (- (* 2 i) 1))
             k)))
tan-cf
Lisp > (define square (lambda (x) (* x x)))
square
Lisp > (iter-a-to-b
  (lambda (k) (tan-cf 1.0 k))
  1
  20)

1->1.000000
2->1.500000
3->1.555556
4->1.557377
5->1.557407
6->1.557408
7->1.557408
8->1.557408
9->1.557408
10->1.557408
11->1.557408
12->1.557408
13->1.557408
14->1.557408
15->1.557408
16->1.557408
17->1.557408
18->1.557408
19->1.557408
20->1.557408
21->1.557408
