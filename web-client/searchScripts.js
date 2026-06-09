function search() {
    var searchType = document.getElementById("searchBy").value;
    var searchVal = document.getElementById("searchValue").value;
    if (searchVal == "") {
        alert("No search value provided. Please provide something to search by.");
        return;
    }


    switch (searchType) {
        case "issueID":
            searchByIssueID(searchVal);
            break;
        case "userID":
            searchByUserID(searchVal);
            break;
        case "status":
            searchByStatus(searchVal);
            break;
        case "priority":
            searchByPriority(searchVal);
            break;
        default:
            alert("ERROR: Unknown search type.");
            break;
    }
}

function searchByIssueID(searchVal) {
    // sanitize by ensuring a number value
    if (isNaN(searchVal)) {
        alert("ERROR: Search value must be a number for Issue ID search.");
        return;
    }

    fetch(`http://localhost:8304/search/issueID/${searchVal}`)
    .then((resp) => resp.json())
    .then(function(data) {
        displayData(data);
    })
    .catch(function(error){
        document.getElementById("result").innerHTML =`No Issues found with id <b>#${searchVal}</b>.`;
    });
}

function searchByUserID(searchVal) {
    // sanitize by ensuring a number value
    if (isNaN(searchVal)) {
        alert("ERROR: Search value must be a number for User ID search.");
        return;
    }

    fetch(`http://localhost:8304/search/userID/${searchVal}`)
    .then((resp) => resp.json())
    .then(function(data) {
        displayData(data);
    })
    .catch(function(error){
        document.getElementById("result").innerHTML =`No Issues found with user id <b>#${searchVal}</b>.`;
    });
}

function searchByStatus(searchVal) {
    fetch(`http://localhost:8304/search/issueStatus/${searchVal}`)
    .then((resp) => resp.json())
    .then(function(data) {
        displayData(data);
    })
    .catch(function(error){
        document.getElementById("result").innerHTML =`No Issues found with status '<b>#${searchVal}</b>'.`;
    });
}

function searchByPriority(searchVal) {
    if (isNaN(searchVal)) {
        alert("ERROR: Search value must be a number for Priority search.");
        return;
    }

    fetch(`http://localhost:8304/search/issuePriority/${searchVal}`)
    .then((resp) => resp.json())
    .then(function(data) {
        displayData(data);
    })
    .catch(function(error){
        document.getElementById("result").innerHTML =`No Issues found with priority '<b>${searchVal}</b>'.`;
    });
}

async function displayData(data) {
    var resultElement = document.getElementById("result");
    var str = "";

    for (const issue of data.issues) {
        const [usersResp, commentsResp] = await Promise.all([
            fetch(`http://localhost:8302/users`).then(r => r.status === 404 ? [] : r.json()),
            fetch(`http://localhost:8303/issues/${issue.id}/comments`).then(r => r.status === 404 ? "" : r.json())
        ]);

        let userName = "Name not found.";
        const foundUser = usersResp.find(u => u.id == issue.user);
        if (foundUser) userName = foundUser.name;

        const comments = {};
        if (Array.isArray(commentsResp)) {
            commentsResp.forEach(c => comments[c.id] = c.text);
        } else {
            // fallback
            issue.comments.forEach(id => comments[id] = "Comment not found.");
        }

        str += `
            <fieldset>
                <legend>#${issue.id}</legend>
                <h3>${issue.title}</h3>
                <span>Priority: ${issue.priority}</span> <br>
                <span>Status: ${issue.status}</span> <br>
                <span>Assigned To: ${issue.user} - ${userName}</span>
                <h4>Comments</h4>
                ${issue.comments.length > 0 
                        ? `<ul>${issue.comments.map(c => `<li>${c}: "${comments[c]}"</li>`).join('')}</ul>`
                        : `<em>No comments</em>`}
            </fieldset>
        `;
    }

    resultElement.innerHTML = str;
}
