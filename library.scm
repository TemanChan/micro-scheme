(define and (lambda args
			  (if (nullp args)
				  1
				  (if (not (car args))
					  0
					  (eval (cons and (cdr args)))))))

(define or (lambda args
			 (if (nullp args)
				 0
				 (if (not (car args))
					 (eval (cons or (cdr args)))
					 1))))

(define > (lambda (x y) (< y x)))
(define >= (lambda (x y) (not (< x y))))
(define <= (lambda (x y) (not (< y x))))
(define = (lambda (x y) (if (< x y) 0 (not (< y x)))))
(define abs (lambda (x) (if (< x 0) (- x) x)))
(define % (lambda (x y) (- x (* (/ x y) y))))
(define ** (lambda (base power) (if (zero? power) 1 (* base (** base (- power 1))))))
(define factorial (lambda (x) (if (< x 2) 1 (* x (factorial (- x 1))))))

;; error if the argument is not an integer
(define even? (lambda (x)
				(if (intp x)
					(if (% x 2)
						0
						1)
					())))

(define odd? (lambda (x)
			   (if (intp x)
				   (if (% x 2)
					   1
					   0)
				   ())))

(define zero? (lambda (x) (if (< x 0) 0 (not (< 0 x)))))

;; general version of for-each: accept any positive number of lists of the same length
(define for-each-get-list (lambda (args)
							(if (nullp args)
								(quote (() ()))
								(let ()
								  (define result (for-each-get-list (cdr args)))
								  (list (cons (car (car args)) (car result)) (cons (cdr (car args)) (car (cdr result))))))))

(define for-each-help (lambda (func lists)
						(define get-list-result (for-each-get-list lists))
						(apply func (car get-list-result))
						(if (nullp (car (car (cdr get-list-result))))
							(quote ())
							(for-each-help func (car (cdr get-list-result))))))

(define for-each (lambda args
				   (for-each-help (car args) (cdr args))))

;; simple version of for-each: accept one list
;;(define for-each (lambda (func lst)
;;				   (if (nullp lst)
;;					   (quote ())
;;					   ((lambda ()
;;						  (func (car lst))
;;						  (for-each func (cdr lst)))))))


(define list (lambda args args))

(define length (lambda (lst)
				   (if (nullp lst)
					   0
					   (+ 1 (list-len (cdr lst))))))

(define list-tail (lambda (lst index)
						 (if (zero? index)
							 lst
							 (if (nullp lst)
								 (print (quote "Error: the index is larger the length of list"))
								 (list-tail (cdr lst) (- index 1))))))

(define list-ref (lambda (lst index) (car (list-tail lst index))))

(define append (lambda (lst1 lst2)
				 (if (nullp lst1)
					 lst2
					 (cons (car lst1) (append (cdr lst1) lst2)))))

(define reverse (lambda (lst)
				  (if (nullp lst)
					  (quote ())
					  (append (reverse (cdr lst)) (list (car lst))))))

(define equal? (lambda (obj1 obj2)
				 (if (or (and (intp obj1) (intp obj2))
						 (and (doublep obj1) (doublep obj2))
						 (and (symbolp obj1) (symbolp obj2)))
					 (= obj1 obj2)
					 (if (and (listp obj1) (listp obj2))
						 (if (and (nullp obj1) (nullp obj2))
							 1
							 (if (or (nullp obj1) (nullp obj2))
								 0
								 (and (equal? (car obj1) (car obj2))
									  (equal? (cdr obj1) (cdr obj2)))))
						 0))))


(define assoc (lambda (key map)
				(if (nullp map)
					0
					(if (equal? key (car (car map)))
						(car map)
						(assoc key (cdr map))))))

(define list-partition (lambda (pred lst)
						 (if (nullp lst)
							 (quote (() ()))
							 (let ()
							   (define result (list-partition pred (cdr lst)))
							   (define less (car result))
							   (define greater (car (cdr result)))
							   (if (pred (car lst))
								   (list (cons (car lst) less) greater)
								   (list less (cons (car lst) greater)))))))

(define list-sort (lambda (less-than? lst)
					(if (nullp lst)
						lst
						(let ()
						  (define pivot (car lst))
						  (define part-result
							(list-partition (lambda (x) (less-than? x pivot)) (cdr lst)))
						  (define sorted-less (list-sort less-than? (car part-result)))
						  (define sorted-greater (list-sort less-than? (car (cdr part-result))))
						  (append sorted-less (cons pivot sorted-greater))))))
