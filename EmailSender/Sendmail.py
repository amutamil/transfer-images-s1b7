import smtplib 

senderMailId = "prajju1998gaargesh@gmail.com"
senderMailpassword = ""
receiverMailId = "prajwalgaargesh@gmail.com"
Subject = "New Image Alert!"

def sendMail(msg):
    email = smtplib.SMTP('smtp.gmail.com', 587) 
    email.starttls() 

    email.login(senderMailId,senderMailpassword) 
    
    Body = msg
    message = 'Subject: {}\n\n{}'.format(Subject, Body)
    
    email.sendmail(senderMailId, receiverMailId, message) 
    email.quit()
