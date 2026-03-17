curl -H 'Content-Type: application/json' \
    -H "Authorization: Bearer eyJhbGciOiJFUzI1NiIsInR5cCI6IkpXVCJ9.MTIzNDU2.hMI8IbgX1yildFcNBgneRHyFfy-tQmfCjjWdyc7wTmqPWv_B-e0PX8Bdo-rnQj3s6TlRU-mbUWrNRwd9H-Qcxw" \
    -d '{
		"name":"Vicente Alvear",
		"email":"linustorvalds@hotmail.com",
		"password":"TUXforTh3W1n"
    }' \
    -X POST \
localhost:8000/auth/signup
