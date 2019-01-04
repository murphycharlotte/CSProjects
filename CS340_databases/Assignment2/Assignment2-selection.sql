#1 
SELECT f.film_id FROM film f
WHERE f.length = (SELECT MAX(film.length) FROM film) AND
f.rental_duration = (SELECT MIN(film.rental_duration) FROM film) AND
f.film_id IS NOT NULL;

#2
SELECT tbl1.name, IFNULL(tbl2.count, 0) as count FROM 
(SELECT c.name AS name from category c) as tbl1
LEFT JOIN 
(SELECT c.name, count(name) AS count
FROM category c INNER JOIN 
film_category fc ON c.category_id = fc.category_id INNER JOIN 
film f ON f.film_id = fc.film_id LEFT JOIN 
film_actor fa on fa.film_id = f.film_id LEFT JOIN 
actor a on a.actor_id = fa.actor_id 
WHERE a.first_name = "ED" AND a.last_name = "CHASE"
GROUP BY c.name) AS tbl2 
ON tbl1.name = tbl2.name;

#3
SELECT tbl1.first_name, tbl1.last_name, IFNULL(tbl2.total,0) AS total FROM
(SELECT a.first_name, a.last_name, a.actor_id AS id FROM actor a) as tbl1
LEFT JOIN
(SELECT a.first_name, a.last_name, a.actor_id AS ID, SUM(f.length) AS total from actor a LEFT JOIN
film_actor fa ON fa.actor_id = a.actor_id LEFT JOIN
film f on f.film_id = fa.film_id LEFT JOIN
film_category fc ON fc.film_id = f.film_id LEFT JOIN
category c ON c.category_id = fc.category_id
WHERE c.name = "Sci-Fi"
GROUP BY a.actor_id) as tbl2
ON tbl1.id = tbl2.id;
#4
SELECT a.first_name, a.last_name FROM actor a INNER JOIN
film_actor fa ON fa.actor_id = a.actor_id INNER JOIN
film f ON f.film_id = fa.film_id INNER JOIN
film_category fc ON fc.film_id = f.film_id INNER JOIN
category c ON c.category_id = fc.category_id
WHERE c.name != "Sci-Fi"
GROUP BY a.actor_id;

#5
SELECT tbl3.title FROM
(SELECT f.title as title, f.film_id AS id FROM film f
) AS tbl3
INNER JOIN
(SELECT tbl1.film_id AS id FROM
(SELECT film_id FROM film_actor fa INNER JOIN
actor a ON a.actor_id = fa.actor_id
WHERE a.first_name = "KIRSTEN" AND a.last_name = "PALTROW") AS tbl1
INNER JOIN
(SELECT film_id FROM film_actor fa INNER JOIN
actor a ON a.actor_id = fa.actor_id
WHERE a.first_name = "WARREN" AND a.last_name = "NOLTE") AS tbl2 
ON tbl1.film_id = tbl2.film_id
) AS tbl4
ON tbl3.id = tbl4.id
ORDER BY title DESC;