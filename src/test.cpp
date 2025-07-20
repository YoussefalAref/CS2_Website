#include <iostream>
#include <fstream>
#include "post.h"
#include "postmanager.h"

void testRetrieveData() {
    // 1. Create a test file
    std::ofstream testFile("posts.txt");
    testFile << "123|456|John Doe|Hello World!|2023-11-15T14:30:00\n";
    testFile << "789|101|Jane Smith|C++ is awesome|2023-11-16T09:15:00\n";
    testFile << " 42 | 99 |  Alice  |  Trim Test  | 2023-11-17T12:00:00  \n"; // Intentional whitespace
    testFile.close();

    // 2. Test the function
    file dbFile;
    std::vector<Post> posts = dbFile.retrieveData();

    // 3. Verify results
    if (posts.size() != 3) {
        std::cerr << "Test Failed: Expected 3 posts, got " << posts.size() << std::endl;
        return;
    }

    // Check first post
    if (posts[0].authorID != 123 || posts[0].postID != 456 || 
        posts[0].author != "John Doe" || posts[0].content != "Hello World!") {
        std::cerr << "Test Failed: First post data mismatch" << std::endl;
        return;
    }

    // Check second post
    if (posts[1].authorID != 789 || posts[1].postID != 101 ||
        posts[1].author != "Jane Smith" || posts[1].content != "C++ is awesome") {
        std::cerr << "Test Failed: Second post data mismatch" << std::endl;
        return;
    }

    // Check whitespace trimming
    if (posts[2].authorID != 42 || posts[2].postID != 99 ||
        posts[2].author != "Alice" || posts[2].content != "Trim Test") {
        std::cerr << "Test Failed: Whitespace trimming not working" << std::endl;
        return;
    }

    std::cout << "All tests passed successfully!" << std::endl;

    // 4. Cleanup (optional)
    remove("posts.txt");
}

int main() {
    testRetrieveData();
    return 0;
}