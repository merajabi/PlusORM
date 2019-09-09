select * from Person as a inner join Book as b on a.id=b.Author;
SELECT * FROM Person as a UNION ALL SELECT * FROM Book as b;



SELECT '' as Person, a.*, '' as Book, b.* FROM Person as a JOIN Book as b ON a.id = b.Author;

