const express = require('express')
const app = express()
const port = 9000
const colors = require('colors');

app.use('/static', express.static('static'))

app.get('/', function(req, res) {
  res.sendFile(__dirname + '/views/index.html')
})
const server = app.listen(port, function () {
  console.log(`Listening on ${server.address().address}:${server.address().port}`);
  console.log("Working".green)
})