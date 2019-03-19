// the login function

var alert_once=false;

function login(event) {
    const username = document.getElementById("username").value; // Get the username from the form
    const password = document.getElementById("password").value; // Get the password from the form

    // Make a URL-encoded string for passing POST data:
    const data = { 'username': username, 'password': password };

    fetch("login.php", {
        method: 'POST',
        body: JSON.stringify(data),
        headers: { 'content-type': 'application/json' }
    })
        .then(response => response.json())
        .then(data => {
            console.log(data.success ? "You've been logged in!" : `You were not logged in ${data.message}`);
            if(data.success) {
                document.getElementById("Signin_Logout").hidden=1;
                document.getElementById("logout_btn").hidden=0;
                document.getElementById("controlscreen").hidden=0;
                document.getElementById("welcome").hidden=0;
                document.getElementById("welcome").textContent = "Welcome, " + data.username;

            }
            else {
                alert("You were not logged in Incorrect Username or Password");
            }
        })
        .catch(error => console.error('Error:',error));
}

document.getElementById("login_btn").addEventListener("click", login, false); // Bind the AJAX call to button click

function signup(event) {
    const username = document.getElementById("username_su").value; // Get the username from the form
    const password = document.getElementById("password_su").value; // Get the password from the form
    if((password === "")||(username === "")) {
        alert("Please input your username or password");
        return;
    }
    // Make a URL-encoded string for passing POST data:
    const data = { 'username': username, 'password': password };

    fetch("register.php", {
        method: 'POST',
        body: JSON.stringify(data),
        headers: { 'content-type': 'application/json' }
    })
        .then(response => response.json())
        .then(data => {
            if(data.success) {
                alert("Sign up success! " + data.message + " as " + data.username);
                console.log(data.message + " as " + data.username);
                document.getElementById("Signin_Logout").hidden=1;
                document.getElementById("logout_btn").hidden=0;
                document.getElementById("controlscreen").hidden=0;
                document.getElementById("welcome").hidden=0;
                document.getElementById("welcome").textContent = "Welcome, " + data.username;

            }
            else {
                console.log(data.message);
                alert("Username already exists!");
            }
        })
        .catch(error => console.error('Error:',error));
}

document.getElementById("signup_btn").addEventListener("click", signup, false);
document.getElementById("logout_btn").addEventListener("click", logout, false);
function logout(event) {
    fetch("logout.php")
        .then(response => response.json())
        .then(data => {
            if(data.success) {
                console.log("Log out success!");
                alert("Log out success!");
            }
            document.getElementById("Signin_Logout").hidden=0;
            document.getElementById("logout_btn").hidden=1;
            document.getElementById("controlscreen").hidden=1;
            document.getElementById("welcome").hidden=1;
        })
        .catch(error => console.error('Error:',error));
}
function warning_Switch(){
  fish.stop_warm();
}


// function time_get(event){
//   var utc = new Date().toJSON().slice(0,10).replace(/-/g,'/');
//   document.getElementById("time_label").innerHTML=utc ;
//   fish.update_feedtime(utc);
// }

//record time of feeding!
function time_get (event) {
    const time = new Date().toJSON().slice(0,10).replace(/-/g,'/');
    const data = { 'time': time };  // 'token': token
    fetch("recordDate.php", {
        method: 'POST',
        body: JSON.stringify(data),
        headers: {'content-type': 'application/json'}
    })
        .then(response => response.json())
        .then(data => {
            console.log(data.message);
            alert(data.message);
            updateCalendar();
            if(data.success) {
                document.getElementById("time_label").value = "";
            }
        })
        .catch(error => console.error('Error:',error));
}
document.getElementById("time_rec").addEventListener("click",time_get,false);

// showing the last feeding time
function show_Date (event) {
    fetch("showDate.php", {
        method: 'POST',
        headers: {'content-type': 'application/json'}
    })
        .then(response => response.json())
        .then(data => {
            if(data.success) {
                const time = data.time;
                console.log(time);
                document.getElementById("time_label").innerHTML = time;
        }
})}
 document.getElementById("time_show_last").addEventListener("click",show_Date,false);


function stateUpdate(newState) {
  console.dir(newState);
  console.log("state update");
  document.getElementById("temperature").innerText=newState.temp+" celsius";
  if(alert_once==true&&newState.temp<30){
    alert_once=false;
  }
  if(newState.alert==true&&alert_once==false){
    alert_once=true;
    alert("temperature too high");
  }
}
function hide_all_page(){
  document.getElementById("controlscreen").hidden=1;
  document.getElementById("welcome").hidden=1;
  document.getElementById("Signin_Logout").hidden=1;
  document.getElementById("logout_btn").hidden=1;
}


function go_to_login(){
  document.getElementById("controlscreen").hidden=1;
  document.getElementById("welcome").hidden=1;
  document.getElementById("Signin_Logout").hidden=0;
  document.getElementById("logout_btn").hidden=1;
}

document.addEventListener("DOMContentLoaded", function(event) {
  go_to_login();
  fish.setStateChangeListener(stateUpdate);
  fish.setup();
});
