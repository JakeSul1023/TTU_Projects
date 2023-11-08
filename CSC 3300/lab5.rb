#run: ruby lab5.rb

require 'mysql2'

begin

#Creates connection to MySQL Server and configures the user information
client = Mysql2::Client.new(host: 'localhost', username: 'root', password: 'coursework')

#Drops TTU database if it exists
client.query("DROP DATABASE IF EXISTS TTU")

#Creates the TTU database
client.query("CREATE DATABASE TTU")

#Selects TTU database to use
client.query("USE TTU")

#Creates the "students" table
client.query("CREATE TABLE students(
  TNumber CHAR(8) PRIMARY KEY, 
  firstname VARCHAR(10) NOT NULL, 
  lastname VARCHAR(10) NOT NULL,
  dateofbirth VARCHAR(10),
  credits DECIMAL(5,2))
  ENGINE=INNODB")

#Inputs the students data into the students tabel
client.query("INSERT INTO students(TNumber, firstname, lastname, dateofbirth, credits)
  VALUES
    ('T1234567', 'Nathan', 'Summers', '3/12/1990', 45),
    ('T5567645', 'Johnny', 'Blaze', '8/15/1972', 36),
    ('T7891011', 'Jean', 'Summers', '9/1/1963', 120),
    ('T7654321', 'Peter', 'Parker', '5/23/1962', 51),
    ('T6677889', 'Wade', 'Wilson', '2/11/1990', 72)")

#Prints the data from the students table
results_students = client.query('SELECT * FROM students')
puts " TNumber   | FirstName  | LastName   | DateOfBirth  | Credits  "
puts "___________|____________|____________|______________|__________"
results_students.each do |row|
  printf("%-10s | %-10s | %-10s | %-12s | %d\n", row['TNumber'], row['firstname'], row['lastname'], row['dateofbirth'], row['credits'])
puts "___________|____________|____________|______________|__________"
end

#Clears memory
results_students.free

#Adds a newline
puts

#Creates the grades table
client.query("CREATE TABLE grades(
  TNumber CHAR(8),
  CourseID CHAR(7),
  Semester VARCHAR(6) CHECK (Semester IN ('Winter', 'Summer', 'Fall', 'Spring')),
  Year INT(4),
  Grade CHAR(1),
  FOREIGN KEY(TNumber) REFERENCES students(TNumber))
  ENGINE=INNODB")
  
#Inputs data into the grades table
client.query("INSERT INTO grades(TNumber, CourseID, Semester, Year, Grade)
  VALUES
    ('T1234567', 'CSC3040', 'Fall', 2013, 'B'),
    ('T7891011', 'MAT1910', 'Fall', 2011, 'A'),
    ('T5567645', 'CSC1300', 'Spring', 2013, 'C'),
    ('T6677889', 'MAT1910', 'Spring', 2012, 'A'),
    ('T7891011', 'CSC1300', 'Spring', 2012, 'F')")

#Displays the grades table with the data
results_grades = client.query('SELECT * FROM grades')
puts "TNumber   | CourseID  | Semester  | Year | Grade"
puts "__________|___________|___________|______|_______"
results_grades.each do |row|
  printf("%-10s| %-9s | %-9s | %4d | %s\n", row['TNumber'], row['CourseID'], row['Semester'], row['Year'], row['Grade'])
puts "__________|___________|___________|______|_______"
end

results_grades.free

client.close
end