Lisp > (define remainder (lambda (a b) (rem a b 0 a)))
remainder
Lisp > (define rem
  (lambda (a b count org_a)
    (cond ((< a b) (- org_a (* b count)))
          (else (rem (- a b) b (+ count 1) org_a)))))
rem
Lisp > (define gcd (lambda (a b)
  (if (= b 0)
      a
      (gcd b (remainder a b)))))
gcd
Lisp > (gcd 206 40)
2
