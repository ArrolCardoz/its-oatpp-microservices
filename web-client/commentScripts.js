// GET
/**
 * @brief Fetches and displays all comments for a given issue.
 *
 * Sends a GET request to the comment microservice to retrieve all comments
 * associated with the specified issue ID. The results are dynamically injected
 * into the commentsContainer element on the page.
 *
 * @param {int} issueID - The ID of the issue whose comments are requested.
 *
 * @details
 * - Shows "No comments." if API returns 404 or an empty list.
 * - Clears the comment display area before new content is loaded.
 * - Displays comments with ID and text in formatted HTML.
 */
function getCommentsByIssue(issueID) {
  fetch("http://localhost:8303/issues/" + issueID + "/comments")
    .then(async (r) => {
      let area = document.getElementById("commentsContainer" + issueID);
      area.innerHTML = "";

      if (!r.ok) {
        area.innerHTML = `<p class="comment">No comments.</p>`;
        return;
      }

      const data = await r.json();

      if (data.length === 0) {
        area.innerHTML = `<p class="comment">No comments.</p>`;
        return;
      }

      data.forEach((c) => {
        area.innerHTML += `
        <li class="comment-text">${c.text}</li>`;
      });
    })
    .catch((err) => console.log("Fetch error:", err));
}
// PUT
/**
 * @brief Updates an existing comment for a given issue.
 *
 * @param {int} issueID - The issue that the comment belongs to.
 *
 * @details
 * - Reads comment text from input element #comment
 * - Reads target comment ID from input element #commentId
 * - On success, displays "Comment Updated!" and clears input
 * - Automatically refreshes comments list via getCommentsByIssue()
 * - Throws an error if the request fails
 */
function editCommentsByIssue(issueID, commentid, comment) {

  fetch("http://localhost:8303/issues/" + issueID + "/comments/" + commentid, {
    method: "PUT",
    headers: {
      "Content-Type": "application/json",
    },
    body: JSON.stringify({ text: comment }),
  })
    .then((r) => {
      if (!r.ok) throw new Error("Failed to update comment");
      getIssueDetails()
    })
    .catch((err) => console.log(err));
}
//POST
/**
 * @brief Adds a new comment to a specific issue.
 *
 * @param {int} issueID - Issue identifier to attach the comment to.
 *
 * @details
 * - Reads new comment from textarea #comment
 * - On success, shows "Comment Added!" and clears input
 * - Throws error if server returns non-200 status
 */
function addCommentsByIssue(issueID, comment) {

  fetch("http://localhost:8303/issues/" + issueID + "/comments", {
    method: "POST",
    headers: {
      "Content-Type": "application/json",
    },
    body: JSON.stringify({ text: comment }),
  })
    .then((r) => {
      if (!r.ok) throw new Error("Failed to add comment");
      getIssueDetails();
    })
    .catch((err) => console.log(err));
}
//DELETE
/**
 * @brief Deletes a comment by its ID from the specified issue.
 *
 * @param {int} issueID - The issue associated with the comment.
 *
 * @details
 * - Reads comment ID from input #commentId
 * - On success, displays "Comment Deleted!"
 * - Automatically refreshes comments list via getCommentsByIssue()
 * - Safe for repeated calls, handles missing comment IDs
 */
function deleteComment(issueID, commentid) {

  fetch(`http://localhost:8303/issues/${issueID}/comments/${commentid}`, {
    method: "DELETE",
  })
    .then((r) => {
      if (!r.ok) throw new Error("Failed to delete comment");
      getIssueDetails();
      getAllIssues();
    })
    .catch((err) => console.log(err));
}
