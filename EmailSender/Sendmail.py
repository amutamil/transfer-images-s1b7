import smtplib 

def sendMail(msg):
    email = smtplib.SMTP('smtp.gmail.com', 587) 
    email.starttls() 
    
    email.login("prajju1998gaargesh@gmail.com", "phm@1998") 
    
    Subject = "New Image Alert!"
    Body = msg
    message = 'Subject: {}\n\n{}'.format(Subject, Body)
    
    email.sendmail("prajju1998gaargesh@gmail.com", "prajwalgaargesh@gmail.com", message) 
    email.quit()
