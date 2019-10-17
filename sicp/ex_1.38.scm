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
Lisp > (iter-a-to-b
  (lambda (k)
          (+
            (cont-frac (lambda (i) 1.0)
                       (lambda (i) (if (= (mod i 3) 2)
                                       (- i (/ (- i 2) 3))
                                       1))
                       k)
            2))
  1
  20)

1->3.000000
2->2.666667
3->2.750000
4->2.714286
5->2.718750
6->2.717949
7->2.718310
8->2.718280
9->2.718284
10->2.718282
11->2.718282
12->2.718282
13->2.718282
14->2.718282
15->2.718282
16->2.718282
17->2.718282
18->2.718282
19->2.718282
20->2.718282
21->2.718282
