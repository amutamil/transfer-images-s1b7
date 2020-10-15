import smtplib 

senderMailId = ""  
senderMailpassword = ""
receiverMailId = ""
Subject = "New Image Alert!"    # Mail Subject

def sendMail(msg):
    email = smtplib.SMTP('smtp.gmail.com', 587) 
    email.starttls() 

    email.login(senderMailId,senderMailpassword) 
    
    Body = msg
    message = 'Subject: {}\n\n{}'.format(Subject, Body)
    
    email.sendmail(senderMailId, receiverMailId, message) 
    email.quit()
