#include <iostream>
#include <cstring>
#include <mysql/mysql.h>

using namespace std;

int main() {
    // MySQL authentication
    const char *server = "localhost";
    const char *user = "root";
    const char *password = "honglong1305";
    const char *database = "long";
    // Message to binary
    const char *message = "Hello, this is Long!";
    unsigned long message_length = strlen(message);

    // Initialize MySQL connection
    MYSQL *conn;
    MYSQL_RES *res;
    MYSQL_ROW row;

    conn = mysql_init(NULL);
    if (conn == NULL) {
        cerr << "mysql_init() failed\n";
        return EXIT_FAILURE;
    }

    // Connect to the database
    if (mysql_real_connect(conn, server, user, password, database, 0, NULL, 0) == NULL) {
        cerr << "mysql_real_connect() failed\n";
        mysql_close(conn);
        return EXIT_FAILURE;
    }

    // Create a query to insert the binary data
    string query = "INSERT INTO 'binary' VALUES (?, ?)";
    MYSQL_STMT *stmt = mysql_stmt_init(conn);
    if (!stmt) {
        cerr << "mysql_stmt_init() failed\n";
        mysql_close(conn);
        return EXIT_FAILURE;
    }

    if (mysql_stmt_prepare(stmt, query.c_str(), query.length())) {
        cerr << "mysql_stmt_prepare() failed: " << mysql_stmt_error(stmt) << "\n";
        mysql_stmt_close(stmt);
        mysql_close(conn);
        return EXIT_FAILURE;
    }

    // Bind the parameters
    MYSQL_BIND bind[2] = {};

    // Bind for message_text (string)
    bind[0].buffer_type = MYSQL_TYPE_STRING;
    bind[0].buffer = (char *)message;
    bind[0].buffer_length = message_length;
    bind[0].is_null = 0;
    bind[0].length = &message_length;

    // Bind for message_binary (binary)
    bind[1].buffer_type = MYSQL_TYPE_BLOB;
    bind[1].buffer = (char *)message;
    bind[1].buffer_length = message_length;
    bind[1].is_null = 0;
    bind[1].length = &message_length;

    if (mysql_stmt_bind_param(stmt, bind)) {
        cerr << "mysql_stmt_bind_param() failed: " << mysql_stmt_error(stmt) << "\n";
        mysql_stmt_close(stmt);
        mysql_close(conn);
        return EXIT_FAILURE;
    }

    // Execute the statement
    if (mysql_stmt_execute(stmt)) {
        cerr << "mysql_stmt_execute() failed: " << mysql_stmt_error(stmt) << "\n";
    } else {
        cout << "Message stored successfully.\n";
    }

    // Clean up
    mysql_stmt_close(stmt);
    mysql_close(conn);
    
    return EXIT_SUCCESS;
}