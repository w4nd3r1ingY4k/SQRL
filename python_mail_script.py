import smtplib
import argparse
from email.mime.text import MIMEText

def send_email(subject, body, to_email):
    gmail_user = "authenticatorsqrl@gmail.com"
    gmail_pw = "nzwh cutl jnvb acqi"

    msg = MIMEText(body)
    msg['Subject'] = subject
    msg['From'] = gmail_user
    msg['To'] = to_email

    try:
        server = smtplib.SMTP_SSL('smtp.gmail.com', 465)
        server.ehlo()
        server.login(gmail_user, gmail_pw)
        server.send_message(msg)
        server.close()

        print('Email Sent!')
    except Exception as e:
        print('Failed to send email: ', e)

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description = 'send eail via gmail')
    parser.add_argument('subject', type=str, help='email subject')
    parser.add_argument('body', type=str, help='Email body')
    parser.add_argument('to_email', type=str, help='recipient email')
    args = parser.parse_args()

    send_email(args.subject, args.body, args.to_email)    
