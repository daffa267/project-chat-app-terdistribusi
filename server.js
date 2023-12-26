const express = require('express');
const mysql = require('mysql2');
const bodyParser = require('body-parser');

const app = express();
const PORT = 3001;

const poolMaster1 = mysql.createPool({
    host: 'localhost',
    user: 'root',
    password: '',
    database: 'pesan',
    connectionLimit: 10,
});

const poolMaster2 = mysql.createPool({
    host: '192.168.1.6',
    user: 'azza',
    password: 'azza',
    database: 'pesan',
    connectionLimit: 10,
});

let activePool = poolMaster1;

app.use(bodyParser.json());

//
function executeQuery(query, values, callback) {
    activePool.query(query, values, (err, results) => {
        if (err) {
            console.error('Error executing query:', err);

            if (activePool === poolMaster1) {
                activePool = poolMaster2;
            } else {
                activePool = poolMaster1;
            }

            console.log('Switching to the other master pool');
            executeQuery(query, values, callback); 
        } else {
            callback(null, results);
        }
    });
}

app.get('/', (req, res) => {
    res.sendFile(__dirname + '/index.html');
});

app.post('/send_message', (req, res) => {
    const { user, message } = req.body;

    executeQuery('INSERT INTO messages (user, message) VALUES (?, ?)', [user, message], (err) => {
        if (err) {
            console.error('Error inserting message:', err);
            res.status(500).json({ status: 'error', message: 'Internal Server Error' });
        } else {
            res.json({ status: 'success', message: 'Message sent successfully' });
        }
    });
});

app.get('/get_messages', (req, res) => {
    executeQuery('SELECT * FROM messages', (err, results) => {
        if (err) {
            console.error('Error getting messages:', err);
            res.status(500).json({ status: 'error', message: 'Internal Server Error' });
        } else {
            res.json({ messages: results });
        }
    });
});

app.get('/register', (req, res) => {
    res.sendFile(__dirname + '/register.html');
});

app.post('/register', (req, res) => {
    const { username, password } = req.body;

    executeQuery('INSERT INTO users (username, password) VALUES (?, ?)', [username, password], (err) => {
        if (err) {
            console.error('Error registering user:', err);
            res.status(500).json({ status: 'error', message: 'Internal Server Error' });
        } else {
            res.redirect('/login');
        }
    });
});

app.get('/login', (req, res) => {
    res.sendFile(__dirname + '/login.html');
});

app.post('/login', (req, res) => {
    const { username, password } = req.body;

    //
    loggedInUser = username;
    res.redirect('/');
});

app.get('/get_logged_in_user', (req, res) => {
    res.json({ username: loggedInUser });
});

app.listen(PORT, () => {
    console.log(`Server is running on http://localhost:${PORT}`);
});
