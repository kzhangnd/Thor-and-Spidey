#!/usr/bin/env python3

import cgi

print('HTTP/1.0 200 OK')
print('Content-Type: text/html')
print('''
<html>
    <head>
        <meta charset="utf-8">
        <meta http-equiv="X-UA-Compatible" content="IE=edge">
        <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
        <link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/bootstrap/4.4.1/css/bootstrap.min.css" integrity="sha384-Vkoo8x4CGsO3+Hhxv8T/Q5PaXtkKtu6ug5TOeNV6gBiFeWPGFN9MuhOf23Q9Ifjh" crossorigin="anonymous">


    </head>
''')

form = cgi.FieldStorage()

if 'overall' and 'availability' and 'helpfulness' and 'usefulness' and 'preparation' \
and 'clarity' and 'fairness' and 'effort' and 'stimulation' and 'promotion' in form:
    if form['overall'].value != 'Excellent' or form['availability'].value != 'Excellent'\
    or form['helpfulness'].value != 'Excellent' or form['usefulness'].value != 'Excellent'\
    or form['preparation'].value != 'Excellent' or form['clarity'].value != 'Excellent'\
    or form['fairness'].value != 'Excellent' or form['effort'].value != 'Excellent'\
    or form['stimulation'].value != 'Excellent' or form['promotion'].value != 'Excellent':

        print('<h4>You are not a good student!!!</h4>')
    else:
        print('<h4>Good Kid! You will do a good job in OS!</h4>')
else:
    print('<h4>Please complete all questions!</h4>')

print('''
<body>
<div class="container mt-5">

    <h1>Course Instructor Evaluation</h1>
    <h2>Survey for Systems Programming<h2>
<form>
<h6>
    <font size="+2">Instructor-Related - Bui, Peter</font><br><br>
    <font size="+1">University Questions</font><br>
    Please consider your overall experience in the course, rather than isolated incidents, when responding to the questions below and throughout the survey. Your individual anonymous, aggregated responses to the following questions will be visible to your instructor, the Department Chair, Dean, and Provost's Office. Rate the named instructor in this course on the following.<br><br>

    About Bui, Peter: Overall organization of the course<br>
    <input type = "radio" name = "overall" value = "Excellent" /> Excellent &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "overall" value = "Very Good" /> Very Good &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "overall" value = "Good" /> Good &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "overall" value = "Satisfactory" /> Satisfactory &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "overall" value = "Poor" /> Poor &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "overall" value = "Very Poor" /> Very Poor &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "overall" value = "Not Applicable" /> Not Applicable<br><br>

    About Bui, Peter: Availability of appropriate help or learning resources outside class<br>
    <input type = "radio" name = "availability" value = "Excellent" /> Excellent &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "availability" value = "Very Good" /> Very Good &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "availability" value = "Good" /> Good &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "availability" value = "Satisfactory" /> Satisfactory &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "availability" value = "Poor" /> Poor &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "availability" value = "Very Poor" /> Very Poor &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "availability" value = "Not Applicable" /> Not Applicable<br><br>

    About Bui, Peter: Helpfulness of required assignments (readings, projects, etc.) in facilitating my learning<br>
    <input type = "radio" name = "helpfulness" value = "Excellent" /> Excellent &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "helpfulness" value = "Very Good" /> Very Good &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "helpfulness" value = "Good" /> Good &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "helpfulness" value = "Satisfactory" /> Satisfactory &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "helpfulness" value = "Poor" /> Poor &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "helpfulness" value = "Very Poor" /> Very Poor &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "helpfulness" value = "Not Applicable" /> Not Applicable<br><br>

    About Bui, Peter: Usefulness of the feedback I received concerning my work in the course<br>
    <input type = "radio" name = "usefulness" value = "Excellent" /> Excellent &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "usefulness" value = "Very Good" /> Very Good &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "usefulness" value = "Good" /> Good &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "usefulness" value = "Satisfactory" /> Satisfactory &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "usefulness" value = "Poor" /> Poor &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "usefulness" value = "Very Poor" /> Very Poor &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "usefulness" value = "Not Applicable" /> Not Applicable<br><br>

    About Bui, Peter: Instructor's preparation for each class<br>
    <input type = "radio" name = "preparation" value = "Excellent" /> Excellent &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "preparation" value = "Very Good" /> Very Good &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "preparation" value = "Good" /> Good &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "preparation" value = "Satisfactory" /> Satisfactory &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "preparation" value = "Poor" /> Poor &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "preparation" value = "Very Poor" /> Very Poor &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "preparation" value = "Not Applicable" /> Not Applicable<br><br>

    About Bui, Peter: Instructor's clarity of communication<br>
    <input type = "radio" name = "clarity" value = "Excellent" /> Excellent &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "clarity" value = "Very Good" /> Very Good &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "clarity" value = "Good" /> Good &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "clarity" value = "Satisfactory" /> Satisfactory &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "clarity" value = "Poor" /> Poor &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "clarity" value = "Very Poor" /> Very Poor &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "clarity" value = "Not Applicable" /> Not Applicable<br><br>

    About Bui, Peter: Instructor's fairness and impartiality in conducting the class<br>
    <input type = "radio" name = "fairness" value = "Excellent" /> Excellent &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "fairness" value = "Very Good" /> Very Good &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "fairness" value = "Good" /> Good &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "fairness" value = "Satisfactory" /> Satisfactory &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "fairness" value = "Poor" /> Poor &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "fairness" value = "Very Poor" /> Very Poor &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "fairness" value = "Not Applicable" /> Not Applicable<br><br>

    About Bui, Peter: Instructor's effort to help me develop mastery of the course material<br>
    <input type = "radio" name = "effort" value = "Excellent" /> Excellent &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "effort" value = "Very Good" /> Very Good &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "effort" value = "Good" /> Good &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "effort" value = "Satisfactory" /> Satisfactory &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "effort" value = "Poor" /> Poor &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "effort" value = "Very Poor" /> Very Poor &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "effort" value = "Not Applicable" /> Not Applicable<br><br>

    About Bui, Peter: Instructor's stimulation of my interest in the subject matter<br>
    <input type = "radio" name = "stimulation" value = "Excellent" /> Excellent &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "stimulation" value = "Very Good" /> Very Good &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "stimulation" value = "Good" /> Good &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "stimulation" value = "Satisfactory" /> Satisfactory &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "stimulation" value = "Poor" /> Poor &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "stimulation" value = "Very Poor" /> Very Poor &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "stimulation" value = "Not Applicable" /> Not Applicable<br><br>

    About Bui, Peter: Overall promotion of my creative, analytical or critical thinking<br>
    <input type = "radio" name = "promotion" value = "Excellent" /> Excellent &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "promotion" value = "Very Good" /> Very Good &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "promotion" value = "Good" /> Good &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "promotion" value = "Satisfactory" /> Satisfactory &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "promotion" value = "Poor" /> Poor &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "promotion" value = "Very Poor" /> Very Poor &nbsp;&nbsp;&nbsp;
    <input type = "radio" name = "promotion" value = "Not Applicable" /> Not Applicable<br><br>

    <input type="submit"><br>
</h6>
</form>
</div>
''')
