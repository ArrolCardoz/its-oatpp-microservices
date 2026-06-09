function getAllUsers() {
    document.getElementById("header").innerHTML = "Users";
    var result;
    fetch('http://localhost:8302/users')
        .then((resp) => resp.json())
        .then((data) => {
            result = "<ul>";
                data.forEach(element => {
                    result += "ID: " + element.id + "<br>";
                    result += "Name: " + element.name + "<br>";
                    result += "Email: " + element.email + "<br>";
                    result += "Role: "
                    switch (element.role) {
                        case 0:
                            result += "Administrator";
                            break;
                        case 1:
                            result += "Developer";
                            break;
                        case 2:
                            result += "Tester";
                            break;
                        case 3:
                            result += "Viewer";
                            break;
                        case 4:
                            result += "Null User";
                            break;
                        default:
                            result += "Undefined";
                            break;
                    }
                    result += "<br>";
                    result += "Assigned Issues: ";
                    element.issues.forEach(id => result += id + ", ");
                    var lastTwo = result.slice(-2);
                    if (lastTwo == ", ") {
                        result = result.slice(0, -2);
                    }
                    result += "<br>";
                    result += "<button type='button' onclick='window.location.href=\"web-client/editUser.html?id=" + element.id + "\"'>Update User</button>";
                    result += "<br>";
                    result += "<button type='button' onclick='deleteUser(" + element.id + ")'>Delete User</button>";
                    result += "<br><br>";
                }); 
            result += "</ul>";
            displayUser(result);
        });
};

function createUser(event) {
    event.preventDefault();

    const username = document.getElementById('name').value;
    const email = document.getElementById('email').value;
    const role = document.querySelector('input[name="role"]:checked')?.value;

    if (!username) {
        alert("Name is required");
        return;
    }

    fetch("http://localhost:8302/users", {
        method: "POST",
        headers: {
            "Content-Type" : "application/json"
        },
        body: JSON.stringify({name: username})
    })
    .then(() => {
        return fetch("http://localhost:8302/users");
    })
    .then(resp => resp.json())
    .then(users => {
        const newID = users.length - 1;
        const body = {};

        if (email !== "") body.email = email;

        if (role !== undefined) {
            switch(role) {
                case "0": 
                body.role = "ADMIN";
                break;

                case "1":
                body.role = "DEVELOPER";
                break;

                case "2":
                body.role = "TESTER";
                break;

                case "3":
                body.role = "VIEWER";
                break;

                default:
                body.role = "VIEWER";
                break;
            }
        }

        if (Object.keys(body).length === 0) {
            window.location.href = "../ITS-Client.html";
            return;
        }

        return fetch('http://localhost:8302/users/' + newID, {
            method: "PUT",
            headers: { "Content-Type": "application/json" },
            body: JSON.stringify(body)
        });
    })
    .then(() => {
        window.location.href = "../ITS-Client.html";
    })
    .catch(err => console.error("Error:", err));
}

function deleteUser(id) {
    if (confirm("Are you sure you wish to delete User #" + id + "?") == true) {
        try {
            fetch('http://localhost:8302/users/' + id, {
                method: "DELETE"
            });
            alert("User #" + id + " deleted.");
            getAllUsers();
        } catch (error) {
            console.error('Network error:', error);
        }
    }
}

function updateUser(event) {
     event.preventDefault();
    const id = getUrlParams();
    const name = document.getElementById("name").value;
    const email = document.getElementById("email").value;
    const role = document.querySelector('input[name="role"]:checked')?.value;

    const body = {};

    if (name !== "")  body.name = name;
    if (email !== "") body.email = email;

    if (role !== undefined) {
        switch(role) {
            case "0": 
            body.role = "ADMIN";
            break;
            case "1":
            body.role = "DEVELOPER";
            break;
            case "2":
            body.role = "TESTER";
            break;
            case "3":
            body.role = "VIEWER";
            break;
            default:
            body.role = "VIEWER";
            break;
        }
    }

    try {
        fetch('http://localhost:8302/users/' + id, {
            method: "PUT",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify(body)
        })
            .then(alert("User #" + id + " updated"))
            .then(window.location.href = "../ITS-Client.html");
    } catch (error) {
        console.error('Network error:', error);
    }
}

function displayUser(result) {
    // Temp placeholder prior to formatting
    if (!result) {
        document.getElementById("result").innerHTML = "No User";
    } else {
        document.getElementById("result").innerHTML = result;
    }
}

function assignUser(issueID, userID) {
    // alert("assignUser() called with issueID " + issueID + " and userID " + userID);
    // TODO: figure out what's needed to:
    //          - add userID to Issue
    //          - update user workload
}

function getUrlParams() {
    const urlParam = new URLSearchParams(window.location.search);
    const receivedValue = urlParam.get('id');
    return receivedValue;
}
