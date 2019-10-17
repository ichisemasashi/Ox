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
          (cont-frac (lambda (i) 1.0)
                     (lambda (i) 1.0)
                     k))
  1
  20)

1->1.000000
2->0.500000
3->0.666667
4->0.600000
5->0.625000
6->0.615385
7->0.619048
8->0.617647
9->0.618182
10->0.617977
11->0.618056
12->0.618026
13->0.618037
14->0.618033
15->0.618034
16->0.618034
17->0.618034
18->0.618034
19->0.618034
20->0.618034
21->0.618034
Lisp > (define cont-frac-re (lambda (n d k)
  (let ((cont-frac-iter (lambda (i result)
    (if (= i k)
        result
        (cont-frac-iter (+ i 1) (/ (n i) (+ (d i) result)))))))
  (cont-frac-iter 1 0))))
cont-frac-re
Lisp > (iter-a-to-b
  (lambda (k)
          (cont-frac-re (lambda (i) 1.0)
                        (lambda (i) 1.0)
                        k))
  1
  20)

1->0
2->1.000000
3->0.500000
4->0.666667
5->0.600000
6->0.625000
7->0.615385
8->0.619048
9->0.617647
10->0.618182
11->0.617977
12->0.618056
13->0.618026
14->0.618037
15->0.618033
16->0.618034
17->0.618034
18->0.618034
19->0.618034
20->0.618034
21->0.618034
