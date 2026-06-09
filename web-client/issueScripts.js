function getAllIssues() {
    document.getElementById("header").innerHTML = "Issues";
    var id;
    var title;
    var userID;
    var status;
    var priority;
    var pString;
    var asString = "";
    try {
        fetch('http://localhost:8301/issues')
            .then((resp) => resp.json())
            .then(function (data) {
                var count = data.length;
                for (i = 1; i < count; i++) {
                    id = data[i]["id"];
                    title = data[i]["title"];
                    userID = data[i]["user"];
                    status = data[i]["status"];
                    priority = data[i]["priority"];
                    switch (priority) {
                        case 1:
                            pString = "Low";
                            break;
                        case 2:
                            pString = "Medium";
                            break;
                        case 3:
                            pString = "High";
                            break;
                        default:
                            pString = "None";
                    }
                    asString += "<fieldset><legend>#" + id + "</legend><h3>" + title + "</h3><span><b>Priority:</b> " + pString + "</span><span><b>Status:</b> " + status + "</span><span><b>Assigned To:</b> User ID #" + userID + "</span><h4>Comments:</h4><ol id='commentsContainer" + id + "'></ol><button type='button' onclick='window.location.href=\"web-client/editIssue.html?id=" + id + "\"'>Update Issue</button><button type='button' onclick='prepIssueForDelete(" + id + ")'>Delete Issue</button></fieldset><br>";
                    getCommentsByIssue(id);
                }
                document.getElementById("result").innerHTML = asString;
            })
    } catch (error) {
        console.error('Network error:', error);
    }
};

function createIssue() {
    let title = prompt("Please enter a title for the Issue");
    try {
        fetch('http://localhost:8301/issues', {
            method: "POST",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify({ title: title })
        });
    } catch (error) {
        console.error('Network error:', error);
    }
    alert('Issue "' + title + '" created');
    getAllIssues();
};

async function fetchOldUser(id) {
    var oldUser;
    try {
        await fetch('http://localhost:8301/issues/' + id)
            .then((resp) => resp.json())
            .then((data) => {
                oldUser = data["user"];
            })
    } catch (error) {
        console.error('Network error:', error);
    }
    return oldUser;
};

async function unassignUser(issueID, userID) {
    try {
        await fetch('http://localhost:8302/user/' + userID + '/workload', {
            method: "PUT",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify({ unassign: [parseInt(issueID)] })
        });
    } catch (error) {
        console.error('Network error:', error);
    }
};

async function updateIssue(event) {
    const intID = parseInt(getUrlParams());

    event.preventDefault();
    const id = getUrlParams();
    const title = document.getElementById("title").value;
    const userElement = document.getElementById("assign");
    const userValue = parseInt(userElement.value);
    const status = document.getElementById("status").value;
    const priorityElement = document.getElementById("priority");
    const priorityValue = parseInt(priorityElement.value);

    const oldUserID = await fetchOldUser(id);

    await unassignUser(id, oldUserID);

    try {
        fetch('http://localhost:8302/user/' + userValue + '/workload', {
            method: "PUT",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify({ assign: [intID] })
        })
    } catch (error) {
        console.error('Network error:', error);
    }

    try {
        fetch('http://localhost:8301/issues/' + id, {
            method: "PUT",
            headers: {
                "Content-Type": "application/json"
            },
            body: JSON.stringify({ title: title, status: status, priority: priorityValue, user: userValue })
        })
            .then(alert("Issue #" + id + " updated"))
            .then(getIssueDetails());
    } catch (error) {
        console.error('Network error:', error);
    }
    window.location.href = "../ITS-Client.html";
}

async function getIssueDetails() {
    const id = getUrlParams();
    var title;
    var user;
    var priority;
    var status;
    var users = ""
    try {
        fetch('http://localhost:8302/users')
            .then((resp) => resp.json())
            .then((data) => {
                for (let i = 0; i < data.length; i++) {
                    users += '<option id="u' + data[i]["id"] + '" value="' + data[i]["id"] + '">' + data[i]["id"] + ' - ' + data[i]["name"] + '</option>';
                }
            })
    } catch (error) {
        console.error('Network error:', error);
    }
    try {
        fetch('http://localhost:8303/issues/' + id + '/comments')
            .then((resp) => {
                if (resp.status === 404) {
                    document.getElementById("comments").innerHTML = "<p>No comments.</p>";
                    return null;
                }

                if (!resp.ok) {
                    throw new Error('HTTP error ' + resp.status);
                }

                return resp.json();
            })
            .then((data) => {
                if (!data) return;
                var comments = "";
                for (let i = 0; i < data.length; i++) {
                    comments += "<fieldset><legend>Comment ID #" + data[i]["id"] + "</legend><p id='c" + data[i]["id"] + "'>" + data[i]["text"] + "</p><button type='button' onclick='updateComment(" + id + ", " + data[i]["id"] + ")'>Update Comment</button><button type='button' onclick='deleteComment(" + id + ", " + data[i]["id"] + ")'>Delete Comment</button></fieldset>"
                }
                document.getElementById("comments").innerHTML = comments;
            })
    } catch (error) {
        console.error('Network error:', error);
    }
    try {
        fetch('http://localhost:8301/issues/' + id)
            .then((resp) => resp.json())
            .then(function (data) {
                title = data["title"];
                user = data["user"];
                status = data["status"];
                priority = data["priority"];
                document.getElementById("title").setAttribute("value", title);
                document.getElementById("assign").setAttribute("value", user);
                document.getElementById(priority).setAttribute("selected", true);
                document.getElementById("status").setAttribute("value", status);
                document.getElementById("assign").innerHTML = users;
                document.getElementById("u" + user).setAttribute("selected", user);
            });
    } catch (error) {
        console.error('Network error:', error);
    }
}

function updateComment(issueID, commentID) {
    var comment = document.getElementById("c" + commentID).innerText;
    const newComment = prompt("What is the updated comment?", comment);
    editCommentsByIssue(issueID, commentID, newComment);
};

async function removeComment(issueID, commentID) {
    deleteComment(issueID, commentID);
};

function addComment() {
    const issueID = getUrlParams();
    const comment = prompt("What is the comment to add?");
    addCommentsByIssue(issueID, comment);
};

async function deleteIssue(id) {
    console.log("deleteIssue() called");
    try {
        fetch('http://localhost:8301/issues/' + id, {
            method: "DELETE"
        });
    } catch (error) {
        console.error('Network error:', error);
    }
};

async function prepIssueForDelete(id) {
    if (confirm("Are you sure you wish to delete issue #" + id + "?") == true) {
        await deleteIssueComments(id) // delete comments attached to the issue
            .then(await removeWorkload(id)); // update user workload

        // Delete the Issue
        await deleteIssue(id)
            .then(alert("Issue Deleted."))
            .then(getAllIssues())
            .then(window.location.reload());
    }
};

async function removeWorkload(id) {
    console.log("removeWorkload() called");
    const userID = await fetchOldUser(id);

    await unassignUser(id, userID);
}

async function deleteIssueComments(issueID) {
    console.log("deleteIssueComments() called");
    try {
        fetch('http://localhost:8303/comments')
            .then((resp) => resp.json())
            .then((data) => {
                for (let i = 0; i < data.length; i++) {
                    if (data[i]["issueId"] == issueID) {
                        deleteComment(issueID, data[i]["id"]);
                    }
                }
            });
    } catch (error) {
        console.error('Network error:', error);
    }
};

function getUrlParams() {
    const urlParam = new URLSearchParams(window.location.search);
    const receivedValue = urlParam.get('id');
    return receivedValue;
}