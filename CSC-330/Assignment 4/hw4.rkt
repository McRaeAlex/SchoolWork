#lang racket

(provide (all-defined-out)) ;; so we can put tests in a second file

;; these definitions are simply for the purpose of being able to run the tests
;; you MUST replace them with your solutions
;;

(define (sequence low high stride)
  (let*
    ([lowstep (+ low stride)]
     [diff (- high low)])
    (cond
      [(positive? diff) (cons low (sequence lowstep high stride))]
      [(zero? diff) (list high)]
      [(negative? diff) (list)])))

(define (string-append-map xs suffix)
  (map (lambda (str) (string-append str suffix)) xs))


(define (list-nth-mod xs n)
  (if [negative? n]
      (error "list-nth-mod: negative number")
      (if (null? xs)
          (error "list-nth-mod: empty list")
          (car (list-tail xs (remainder n (length xs)))))))

(define (stream-for-n-steps s n)
  (if (positive? n)
      (cons (car (s)) (stream-for-n-steps (cdr (s)) (- n 1)))
      (list)))

(define funny-number-stream
  (letrec
      ([f (lambda (x)
            (if (zero? (remainder x 5))
                (cons (* -1 x) (lambda () (f (+ x 1))))
                (cons x (lambda () (f (+ x 1))))))])
    (lambda () (f 1))))

(define cat-then-dog
  (lambda () (cons "cat.jpg" (lambda () (cons "dog.jpg" cat-then-dog)))))

(define stream-add-zero
  (lambda (s)
    (lambda ()
      (cons (cons 0 (car (s))) (stream-add-zero (cdr (s)))))))


(define (cycle-lists xs ys)
  (local [(define inner (lambda (xs ys ix iy)
            (lambda ()
              (cons (cons (list-nth-mod xs ix) (list-nth-mod ys iy))
                    (inner xs ys (+ ix 1) (+ iy 1))))))]
    (inner xs ys 0 0)))

(define (vector-assoc v vec)
  (local [(define (inner v vec i)
                          (if (<= (vector-length vec) i)
                              false
                              (let [(vecati (vector-ref vec i))]
                                    (if (pair? vecati)
                                        (if (equal? (car vecati) v)
                                            vecati
                                            (inner v vec (+ i 1)))
                                        (inner v vec (+ i 1))))))]
    (inner v vec 0)))

(define (cached-assoc xs n)
  (let [
        (cache (make-vector n false))
        (index 0)]
    (lambda (v)
      (let [(cache-check (vector-assoc v cache))]
        (if (pair? cache-check)
            (cache-check)
            (let [(val (assoc v xs))]
              (if (pair? val)
                   (begin
                     (vector-set! cache index val)
                     (set! index (+ index 1))
                     val) ; unsure if brackets are correct
                  false)))))))


(define-syntax while-less
  (syntax-rules (do)
    [(while-less e1 do e2)
     (letrec [(first-cache e1)
           (g (lambda ()
                (if (< e2 first-cache)
                    (g)
                    true)))]
       (g))]))