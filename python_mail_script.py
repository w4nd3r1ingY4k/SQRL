import smtplib
import argparse
import os
from email.mime.text import MIMEText


def send_email(subject, body, to_email):
    
    # IMPORTANT: don't hardcode credentials in source control.
    # Configure these via environment variables:
    # - SQRL_GMAIL_USER (default: authenticatorsqrl@gmail.com)
    # - SQRL_GMAIL_APP_PASSWORD (required)
    gmail_user = os.environ.get("SQRL_GMAIL_USER", "authenticatorsqrl@gmail.com")
    gmail_pw = os.environ.get("SQRL_GMAIL_APP_PASSWORD")
    if not gmail_pw:
        raise RuntimeError(
            "Missing SQRL_GMAIL_APP_PASSWORD env var. "
            "Set it to a Gmail App Password before sending mail."
        )
    

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
