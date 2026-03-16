curl -H 'Content-Type: application/json' \
    -H "Authorization: Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpZCI6Mn0=.wmthwzlK3LyvDyHSlDSY2dsdv50PjqDGO_KFGj-rYXs=" \
    -d '{
		"name":"Vicente Alvear",
		"email":"linustorvalds@hotmail.com",
		"password":"TUXforTh3W1n"
    }' \
    -X POST \
localhost:8000/auth/signup
