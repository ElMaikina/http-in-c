curl -H 'Content-Type: application/json' \
    -d '{
		"name":"Linus Torvalds",
		"email":"linustorvalds@hotmail.com",
		"password":"TUXforTh3W1n"
    }' \
    -X POST \
localhost:8000/users
