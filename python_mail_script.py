import smtplib
import argparse
import os
from email.mime.text import MIMEText


def _load_dotenv_if_present(dotenv_path: str) -> None:
    """
    Minimal .env loader (no external dependency).
    - Only sets keys that are not already present in os.environ
    - Supports lines like KEY=VALUE (optionally quoted)
    - Ignores blank lines and comments starting with '#'
    """
    if not os.path.exists(dotenv_path):
        return

    with open(dotenv_path, "r", encoding="utf-8") as f:
        for raw in f:
            line = raw.strip()
            if not line or line.startswith("#"):
                continue
            if "=" not in line:
                continue

            key, value = line.split("=", 1)
            key = key.strip()
            value = value.strip().strip('"').strip("'")
            if key and key not in os.environ:
                os.environ[key] = value


def send_email(subject, body, to_email):
    
    # IMPORTANT: don't hardcode credentials in source control.
    # Configure these via environment variables:
    # - SQRL_GMAIL_USER (default: authenticatorsqrl@gmail.com)
    # - SQRL_GMAIL_APP_PASSWORD (required)
    script_dir = os.path.dirname(__file__)

    # Allow a local ".env" next to this script (gitignored).
    _load_dotenv_if_present(os.path.join(script_dir, ".env"))

    gmail_user = os.environ.get("SQRL_GMAIL_USER", "authenticatorsqrl@gmail.com")
    gmail_pw = os.environ.get("SQRL_GMAIL_APP_PASSWORD")
    if not gmail_pw:
        # Restore "just run it" behavior without committing secrets:
        # allow a local file next to this script (gitignored).
        secret_path = os.path.join(script_dir, "gmail_app_password.txt")
        try:
            with open(secret_path, "r", encoding="utf-8") as f:
                gmail_pw = f.read().strip()
        except FileNotFoundError:
            gmail_pw = None

    if not gmail_pw:
        raise RuntimeError(
            "Missing Gmail app password. Set SQRL_GMAIL_APP_PASSWORD or create "
            "gmail_app_password.txt next to python_mail_script.py (one line)."
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
