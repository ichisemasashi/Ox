Lisp > (define filtered-acc (lambda (combiner null-value filter-pre term a next b)
  (cond ((> a b) null-value)
        ((filter-pre a)
         (combiner
           (term a)
           (filtered-acc combiner null-value filter-pre term (next a) next b)))
        (else (filtered-acc combiner null-value filter-pre term (next a) next b)))))
filtered-acc
Lisp > (define sum-squared-primes (lambda (a b)
  (filtered-acc + 0 prime? square a inc b)))
sum-squared-primes
Lisp > (define square (lambda (x) (* x x)))
square
Lisp > (define inc (lambda (n) (+ n 1)))
inc
Lisp > (define prime? (lambda (n)
  (= n (smallest-divisor n))))
prime?
Lisp > (define smallest-divisor (lambda (n)
  (find-divisor n 2)))
smallest-divisor
Lisp > (define find-divisor (lambda (n test-divisor)
  (cond ((> (square test-divisor) n) n)
        ((divides? test-divisor n) test-divisor)
        (else (find-divisor n (+ test-divisor 1))))))
find-divisor
Lisp > (define divides? (lambda (a b) (= (mod b a) 0)))
divides?
Lisp > (sum-squared-primes 1 10)
88
Lisp > (define term- (lambda (i) i))
term-
Lisp > (define and (lambda (x y)
  (cond ((= x true) (cond ((= y true) true)
                    (else false)))
        (else false))))
and
Lisp > (define gcd (lambda (a b)
  (if (= b 0)
      a
      (gcd b (mod a b)))))
gcd
Lisp > (define product-with-gcd (lambda (n)
  (let ((p (lambda (i)
           (and (> n i) (= (gcd i n) 1)))))
  (filtered-acc * 1 p term- 1 inc n))))
product-with-gcd
Lisp > (product-with-gcd 10)
189
Lisp > (define filtered-acc-re (lambda (combiner null-value filter-pre term a next b)
  (filtered-acc-iter combiner null-value filter-pre term a next b)))
filtered-acc-re
Lisp > (define filtered-acc-iter (lambda (combiner null-value filter-pre term a next b)
  (cond ((> a b) null-value)
        ((filter-pre a)
         (filtered-acc-iter combiner
                            (combiner (term a) null-value)
                            filter-pre
                            term
                            (next a)
                            next
                            b))
        (else (filtered-acc-iter combiner null-value filter-pre term (next a) next b)))))
filtered-acc-iter
Lisp > (define sum-primes2 (lambda (a b)
  (filtered-acc-re + 0 prime? square a inc b)))
sum-primes2
Lisp > (sum-primes2 1 10)
88
Lisp > (define product-with-gcd-re (lambda (n)
  (let ((p (lambda (i) (and (> n i) (= (gcd i n) 1)))))
    (filtered-acc-re * 1 p term- 1 inc n))))
product-with-gcd-re
Lisp > (product-with-gcd-re 10)
189
