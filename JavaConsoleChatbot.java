import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;

/**
 * Java Console Chatbot
 * ---------------------
 * Project: Java Console Chatbot
 * Author : Omkar Dhekane
 *
 * A rule-based console chatbot built with Core Java. All questions and their
 * corresponding answers are stored in-memory (no external APIs/databases).
 * The response-handling architecture is modular: new rules can be added to
 * the responses map without touching the core matching logic.
 */
public class JavaConsoleChatbot {

    private final Map<String, String> responses = new HashMap<>();

    public JavaConsoleChatbot() {
        loadRules();
    }

    /** All chatbot rules live here — add a new keyword/response pair any time. */
    private void loadRules() {
        responses.put("hi", "Hello! How can I help you today?");
        responses.put("hello", "Hi there! What would you like to know?");
        responses.put("how are you", "I'm just a program, but I'm running smoothly. How about you?");
        responses.put("your name", "I'm a simple Java console chatbot, built as a learning project.");
        responses.put("bye", "Goodbye! Have a great day.");
        responses.put("help", "You can say things like: hi, hello, how are you, your name, bye.");
        responses.put("default", "Sorry, I didn't understand that. Type 'help' to see what I can respond to.");
    }

    /** Very simple keyword matching against the stored rules. */
    private String getResponse(String userInput) {
        String input = userInput.toLowerCase().trim();

        for (String keyword : responses.keySet()) {
            if (!keyword.equals("default") && input.contains(keyword)) {
                return responses.get(keyword);
            }
        }
        return responses.get("default");
    }

    public void run() {
        Scanner scanner = new Scanner(System.in);
        System.out.println("Chatbot: Hello! Type 'bye' anytime to exit. Type 'help' for a list of things I understand.");

        while (true) {
            System.out.print("You: ");
            String userInput = scanner.nextLine();

            String reply = getResponse(userInput);
            System.out.println("Chatbot: " + reply);

            if (userInput.toLowerCase().trim().contains("bye")) {
                break;
            }
        }
        scanner.close();
    }

    public static void main(String[] args) {
        new JavaConsoleChatbot().run();
    }
}
